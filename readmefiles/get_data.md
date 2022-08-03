<h1>42 Labs 3º Edição</h1>

### Desafio : Aplicação de monitoramento de serviços web.

<h1></h1>

A função `get_data` armazena as informações disponibilizadas pelo arquivo monitoring.db em structs.

```c
int	get_data(t_config *config)
{
	int			fd;
	int			data_result;
	char		*buffer;
	char		**split;
	t_config	*temp;
	
	//Abre o arquivo para leitura.
	fd = open("monitoring.db", O_RDONLY);

	//retorna em caso de erro.
	if (fd == -1)
	{
		printf("Error: Could not open file\n");
		return (1);
	}

	//A função get_next_linemod retorna a próxima linha do arquivo, removendo o \n final;
	buffer = get_next_linemod(fd);

	//Ignora string especificada.
	if (!strcmp(buffer, "# monitoring.db"))
	{
		free(buffer);
		buffer = get_next_linemod(fd);
	}
	temp = config;

	//Perpetua loop enquanto houverem linhas a serem lidas no arquivo.
	while (buffer)
	{
		//Ignora linhas vazias.
		if (buffer[0] != '\0')
		{
			//divide termos separados por tabulação.
			split = ft_split(buffer, '\t');

			//Verifica se os dados contidos na linha são válidos.
			data_result = check_data(split);

			//Interrompe processo em caso de informação invalida.
			if (data_result == -1)
			{
				printf("Error: Could not get data\n");
				close(fd);
				return (1);
			}

			//Preenche o struct com as informações presentes na linha
			fill_data(temp, split);

			free(buffer);
			ft_freeptr(split);

			//Aloca memória e inicializa as variaveis a serem utilizada para armazena as informações da próxima linha.
			temp->next = malloc(sizeof(*config));
			config_init(temp->next);
			temp = temp->next;
		}
		else
			free(buffer);
		buffer = get_next_linemod(fd);
	}
	close(fd);
	return (0);
}
```

<h1></h1>

As structs utilizadas para armazenar as informações relevantes ao monitoramento seguem o seguinte formato

```c
typedef struct	s_config
{
	//Protocolo utilizado para análise: 1->HTTP, 2->PING, 3->DNS
	int					protocol;

	//Nome atribuido ao teste.
	char				*name;

	//Endereço a ser verificado;
	char				*address;

	//Intervalo de tempo em segundos entre cada monitoramento
	unsigned long long	interval;

	//String contendo o servidor DNS a ser verificado ou o método HTTP a ser utilizado.
	char				*DNSserver_HTTPtmethod;

	//Código HTTP esperado;
	long				HTTPcode;

	//Momento em que foi feito o último monitoramento.
	unsigned long long	last_check;

	//Ponteiro para o processo inicialido por curl;
	CURL				*curl_handle;

	//Ponteiro para o struct referente a outro serviço a ser testado.
	struct s_config		*next;
}						t_config;
```