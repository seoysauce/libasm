#include "libasm.h"
int	main(void)
{
	char s[500];
	const char *s1 = "aaa";
	const char *s2 = "aah";
	char buff[10];

	printf("_______________<ft_strlen>_______________\n");
	printf("[case 1] ft_strlen = %zu\n", ft_strlen("hello")); // 5
	printf("[case 2] ft_strlen = %zu\n", ft_strlen(""));	// 0
	printf("[case 3] ft_strlen = %zu\n", ft_strlen("the\0hidden"));   // 3
	printf("[case 4] ft_strlen = %zu\n", ft_strlen("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));   // 3
	printf("         strlen = %zu\n", strlen("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));   // 3

	printf("_______________<ft_strcmp>_______________\n");
	printf("[case 1] ft_strcpy = %s\n", ft_strcpy(s, "hello")); // hello
	printf("[case 2] ft_strcpy = %s\n", ft_strcpy(s, "")); //
	printf("[case 3] ft_strcpy = %s\n", ft_strcpy(s, "the\0hidden"));  // the
	printf("         strcpy = %s\n", strcpy(s, "the\0hidden"));  // the
	printf("[case 4] ft_strcpy = %s\n", ft_strcpy(s, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));  // the
	printf("         strcpy = %s\n", strcpy(s, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));  // the

	printf("_______________<ft_strcmp>_______________\n");
	printf("[case 1] ft_strcmp = %d\n", ft_strcmp(s1, s2)); // -7
	printf("         strcmp = %d\n", strcmp(s1, s2)); // -7 문자 리터럴 주면 (-1, 0, 1) 로 최적화
	printf("[case 2] ft_strcmp = %d\n", ft_strcmp("", ""));
	printf("         strcmp = %d\n", strcmp("", ""));
	printf("[case 3] ft_strcmp = %d\n", ft_strcmp("", s2));
	printf("         strcmp = %d\n", strcmp("", s2));
	printf("[case 4] ft_strcmp = %d\n", ft_strcmp(s1, ""));
	printf("         strcmp = %d\n", strcmp(s1, ""));
	// printf("[case 5] ft_strcmp = %d\n", ft_strcmp(s1));
	// printf("         strcmp = %d\n", strcmp(s1));
	// printf("[case 5] ft_strcmp = %d\n", ft_strcmp(s1, s2, ""));
	// printf("         strcmp = %d\n", strcmp(s1, s2, ""));

	printf("_______________<ft_write>_______________\n");
	printf("[case 1] ft_write = %zd\n", ft_write(1, "HelloWorld\n", 11));
	printf("         write = %zd\n", write(1, "HelloWorld\n", 11));
	printf("[case 2] ft_write = %zd\n", ft_write(1, "the\0hidden\n", 10));
	printf("         write = %zd\n", write(1, "the\0hidden\n", 10));
	printf("[case 3] ft_write = %zd, errno = %d, msg = %s\n", write(-3, "HelloWorld\n", 10), errno, strerror(errno));
	errno = 0;
	printf("         write = %zd, errno = %d, msg = %s\n", write(-3, "HelloWorld\n", 10), errno, strerror(errno));
	int fd;
	fd = open("hello.txt",O_RDWR);
	printf("[case 4] ft_write = %zd\n", ft_write(fd, "HelloWorld\n", 11));
	printf("         write = %zd\n", ft_write(fd, "HelloWorld\n", 11));
	close(fd);
	fd = open("hello.txt",O_RDWR);
	printf("_______________<ft_read>_______________\n");
	printf("[case 1] ft_read = %zd\n", ft_read(0, s, 20));
	printf("         read = %zd\n", read(0, s, 20));
	printf("[case 2] ft_read = %zd, buff = %s\n", ft_read(fd, buff, 5), buff);
	printf("         read = %zd, buff = %s\n", read(fd, buff, 5), buff);

	printf("_______________<ft_strdup>_______________\n");
	printf("[case 1] ft_strdup = %s\n", ft_strdup("HelloWorld"));
	printf("         strdup = %s\n", strdup("HelloWorld"));
	printf("[case 2] ft_strdup = %s\n", ft_strdup("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));
	printf("         strdup = %s\n", strdup("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));
	return (1);
}
