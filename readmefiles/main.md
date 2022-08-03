<h1>42 Labs 3º Edição</h1>

### Desafio : Aplicação de monitoramento de serviços web.

<h1></h1>

A função `main` chama as demais funções desse projeto com base nos argumentos enviado pelo usuário e inicializa variáveis necessarias

```c
int	main(int argc, char **argv)
{
	t_config	*config;
	int			log_file;
	FILE		*temp_log;
	
	//Chama a função simplify_log quando o argumento --simplify é utilizado.
	if (argc == 2 && strstr(argv[1], "--simplify"))
		simplify_log();

	//Executa o monitoramento quando a função é chamada sem argumentos.
	else if (argc == 1)
	{
		//Aloca memória para a primeira struct config, utilizadas para armazenar dados referente a cada serviço a ser 
		//monitorado.
		config = malloc(sizeof(*config));
		
		//log_file será utilizado para armazenar informações no arquivo monitoring.log.
		if (access("monitoring.log", F_OK) == 0)
			log_file = open("monitoring.log", O_APPEND | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		else
			log_file = open("monitoring.log", O_CREAT | O_WRONLY | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

		//temp_log será utilizado para armazenar temporariamente informações no arquivo temp_log.txt, que será utilizado para
		//imprimir as informações relevantes no arquivo monitoring.log e no terminal.
		temp_log = fopen("temp_log.txt", "w");

		//inicializa variaveis da struct config;
		config_init(config);

		//A função get_data lê os dados presentes no arquivo monitoring.db e armazena-os para que possam ser utilizados pelo //programa.
		//A função curl_setup inicializa o curl com as opções adequadas para cada tipo de teste;
		//A função ft_exit libera a memoria utilizada pelo programa e o interrompe.
		if (get_data(config) || curl_setup(config, temp_log))
			ft_exit(config, temp_log, log_file);

		//Modifica o comportamento de SIGINT para que mude o de uma variável global, utilizado para terminar o loop de 
		//monitoramento
		signal(SIGINT, ft_stop);

		//realiza o monitoramento até que o processo seja interrompido.
		monitoring_loop(config, temp_log, log_file);

		ft_exit(config, temp_log, log_file);
	}
	else
		printf("Error: Invalid arguments\n");
	return (0);
}
```

<h1></h1>

A função simplify_log lê o arquivo monitoring.log e imprime seu conteudo de forma simplificada no terminal.

```c
void	simplify_log(void)
{
	FILE		*log_file;
	char		buffer[100];

	//Abre o arquivo monitoring.log.
	log_file = fopen("monitoring.log", "r");
	if (log_file)
	{
		//Lê o arquivo linha a linha até seu fim.
		while (fgets(buffer, 100, log_file))
		{
			//Procura palavras chaves e imprime as linhas relevante.
			if (strstr(buffer, "LOG"))
			{
				printf("\n--------------------------------------\n\n");
				printf("%s", buffer);
			}
			else if (strstr(buffer, "HTTP") || strstr(buffer, "Host:") || strstr(buffer, "Accept:"))
				printf("%s", buffer);
			else if (strstr(buffer, "SUCCESS") || strstr(buffer, "FAILED") || strstr(buffer, "Error"))
			{
				printf("%s", buffer);
				printf("\n--------------------------------------\n");
			}
		}
	}
	//fecha o arquivo.
	fclose(log_file);
}
```