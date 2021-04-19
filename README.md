# SOR-Semáforos-1S-2021

#### PSEUDOCODIGO


*Semaforos:*

	s_mezclar
	s_salar
	s_salero (mutex)
	s_agregarCarne
	s_empanar
	s_fritar
	s_sarten (mutex)
	s_horno (contador)
	s_requisito1 (hacer la milanesa)
	s_requisito2 (honear los panes)
	s_requisito3 (cortar las verduras)

*Acciones:*

	cortar()
	mezclar()
	salar()
	agregarCarne()
	empanar()
	fritar()
	hornear()
	cortarVegetales()
	armar()
	
*Prueba de escritorio:*

	 cortar()
	v(s_mezclar)
	
	p(s_mezclar)
	 mezclar()
	v(s_salar)
	
	p(s_salar)
	p(s_salero)
	 salar()
	v(s_salero)
	v(s_agregarCarne)
	
	p(s_agregarCarne)
	 agregarCarne()
	v(s_empanar)
	
	p(s_empanar)
	 empanar()
	v(s_fritar)
	
	p(s_fritar)
	p(s_sarten)
	 fritar()
	v(s_sarten)
	v(s_requisito1)
	
	p(s_horno)
	 hornear()
	v(horno)
	v(s_requisito2)
	
	 cortarExtras()
	v(s_requisito3) 
	

#### CODIGO


*El codigo harcodeado:*

	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");

	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
	strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
	strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");

	strcpy(pthread_data->pasos_param[2].accion, "salar");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "sal");

	strcpy(pthread_data->pasos_param[3].accion, "agregarCarne");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "carne");

	strcpy(pthread_data->pasos_param[4].accion, "empanar");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "carne");

	strcpy(pthread_data->pasos_param[5].accion, "fritar");
	strcpy(pthread_data->pasos_param[5].ingredientes[0], "milanesa");

	strcpy(pthread_data->pasos_param[6].accion, "hornear");
	strcpy(pthread_data->pasos_param[6].ingredientes[0], "pan");

	strcpy(pthread_data->pasos_param[7].accion, "cortarExtras");
	strcpy(pthread_data->pasos_param[7].ingredientes[0], "lechuga");
	strcpy(pthread_data->pasos_param[7].ingredientes[1], "tomate");
	strcpy(pthread_data->pasos_param[7].ingredientes[2], "cebolla");
	strcpy(pthread_data->pasos_param[7].ingredientes[3], "pepino");

	strcpy(pthread_data->pasos_param[8].accion, "armado");
	strcpy(pthread_data->pasos_param[8].ingredientes[0], "milanesa");
	strcpy(pthread_data->pasos_param[8].ingredientes[1], "pan");
	strcpy(pthread_data->pasos_param[8].ingredientes[2], "vegetales");

*Fue reemplazado por:*

	FILE * receta = fopen("receta.txt", "r"); /*abrimos la receta para leerla*/

	int a=0;
	int b=0;
	bool esAccion=true;

	char texto[50];

	while(feof(receta) == 0){ /*recorremos todo el archivo*/
		fscanf(receta, "%s", texto); /*con fscanf agarramos una linea hasta \n y la insertamos en la variable texto*/

		for (char *palabra = strtok(texto, "|"); palabra; palabra = strtok(NULL, "|")){ /*spliteamos texto por "|" para separar las palabras*/

			if(esAccion==true){ /*sabemos que la primera palabra es siempre una accion*/
				strcpy(pthread_data->pasos_param[a].accion, palabra); /*insertamos la palabra*/
				esAccion=false; /*ponemos la variable en false ya que las demas palabras son ingredientes*/

			}else{
				strcpy(pthread_data->pasos_param[a].ingredientes[b], palabra);
				b++;/*incrementamos para poder poner el seiguiente ingrediente*/
			}
		}

		a++; /*aumentamos los pasos*/
		b=0; /*reseteamos los ingredientes*/
		esAccion=true; /*reseteamos el valor de esAccion ya que vamos a recorrer el sigueinte paso*/
	}
	fclose(receta);

#### ACLARACIONES
1)Los comentarios del codigo original fueron borrados debido a que estaban escritos de la forma:
	
	//comentario
	
y no de la forma:
	
	/*comentario*/

lo cual me daba error al compilar y por eso decidi eliminarlos

2)Las demas funciones agregadas estan explicadas dentro del codigo
