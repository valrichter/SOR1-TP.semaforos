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
	s_requisito1
	s_requisito2
	s_requisito3

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
	
*Codigo:*

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
	
	
	
	
