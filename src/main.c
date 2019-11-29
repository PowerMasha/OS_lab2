#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc, char **argv){
  int f;
  char buf[160];
  int fd[2];
  pid_t pid; // создание процесса
  if (pipe(fd) < 0){
        printf("Cant create pipe\n");
        return -1;
    }//создание файла
  if ((f = open(argv[1], O_RDWR | O_CREAT | O_APPEND)) == -1){ 
    printf("Cannot open file.\n");
    return -1;
  }
  gets(buf);
  pid = fork();
  if(pid == -1){
    printf("Can\'t fork\n"); // процесс не создался
    return -1;
  }//ребенок
  else if(pid == 0){
    close(fd[1]);//закрытие входа
    char c;
    while (read(fd[0], &c, sizeof(char)) == sizeof(char)) { // чтение команд 
      write(f, &c, sizeof(char));
    }
    close(fd[0]);
  }//родитель
  else{
    close(fd[0]);// закрытие выхода
    if (dup2(fd[1], 1) < 0) { // замена стандартного вывода на вход в пайп
            printf("can`t dup2\n");
            exit(-1);
    }
    execlp(buf, buf, NULL); //выполнение
    close(fd[1]);
  }
  return 0;
}
