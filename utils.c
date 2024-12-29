/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ealmonte <ealmonte@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 20:00:33 by decortejohn       #+#    #+#             */
/*   Updated: 2024/12/23 18:09:24 by ealmonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
 * exit_handler: Maneja la salida del programa.
 * Si el código de salida es 1, imprime un mensaje de uso correcto.
 * Luego termina la ejecución del programa con exit(0).
 */
void	exit_handler(int n_exit)
{
	if (n_exit == 1)
		ft_putstr_fd("./pipex infile cmd cmd outfile\n", 2);
	exit(0);
}

/*
 * open_file: Abre un archivo en modo de lectura o escritura según el parámetro
 in_or_out.
 * Si in_or_out es 0, abre el archivo en modo solo lectura (O_RDONLY).
 * Si in_or_out es 1, abre el archivo en modo escritura (O_WRONLY) con permisos
 * de creación o truncado (O_CREAT | O_TRUNC).
 * Devuelve el descriptor de archivo si tiene éxito, o termina el programa si
 * hay un error (retorna -1).
*/

int	open_file(char *file, int in_or_out)
{
	int	ret;

	if (in_or_out == 0)
		ret = open(file, O_RDONLY, 0777);
	if (in_or_out == 1)
		ret = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (ret == -1)
		exit(0);
	return (ret);
}

/*
 * ft_free_tab: Libera la memoria de un array de cadenas de caracteres.
 * Recorre el array liberando cada elemento y luego el array mismo.
 */
void	ft_free_tab(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/*
 * my_getenv: Busca una variable de entorno por su nombre en el array de
 variables de entorno.
 * Si la encuentra, devuelve el valor de la variable (después del igual), de lo
 contrario, devuelve NULL.
 */
char	*my_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strcmp(sub, name) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}

/*
 * get_path: Busca el comando dentro de los directorios especificados en la
 variable de entorno PATH.
 * Divide el valor de PATH en partes usando ':' como delimitador.
 * Luego, por cada directorio, intenta construir la ruta completa al comando y
 * verificar si existe y es ejecutable.
 * Si encuentra el comando, devuelve la ruta completa; si no, devuelve el
 * comando tal cual.
 */
char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	**s_cmd;

	i = -1;
	allpath = ft_split(my_getenv("PATH", env), ':');
	s_cmd = ft_split(cmd, ' ');
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, s_cmd[0]);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			ft_free_tab(s_cmd);
			return (exec);
		}
		free(exec);
	}
	ft_free_tab(allpath);
	ft_free_tab(s_cmd);
	return (cmd);
}
