#include <string.h>
#include <fstream>
#include <unistd.h>
#include <cmath>
#define _USE_MATH_DEFINE

using namespace std;
char espacio = ' ';
void histograma(char *filename, int tramos, char *destino){

	//Se declaran variables
	unsigned int altura=0, anchura=0;
	
	FILE * ifp;

	//Se abre el fichero
   	ifp = fopen(filename, "rb");

	//Control de error de fichero
   	if (!ifp)
	{
        	exit(-1);
    	}
    
	//Lectura de fichero
   	size_t sizeRead0 = fread(&altura, sizeof(int), 1, ifp );
    	if(sizeRead0 ==0){exit(-1);}

    	size_t sizeRead1 = fread(&anchura, sizeof(int), 1, ifp );
    	if(sizeRead1 ==0){exit(-1);}
    
	//Declaracion de variables para las matrices de colores
	unsigned int longitud = altura * anchura;
	unsigned int total= (int)(longitud/sizeof(char));
	
	unsigned char* rojo = new unsigned char[longitud];
	unsigned char* verde = new unsigned char[longitud]; 
	unsigned char* azul = new unsigned char[longitud]; 
	unsigned char* gris = new unsigned char[longitud]; 
	

	//Se leen los colores, guardando los valores en la matriz correspondiente
        size_t sizeRead2 = fread(&rojo[0], sizeof( char), total, ifp );
        if(sizeRead2 !=total){exit(-1);}

	size_t sizeRead3 = fread(&verde[0], sizeof(char), total, ifp );
        if(sizeRead3!=total){exit(-1);}

	size_t sizeRead4 = fread(&azul[0], sizeof(char), total, ifp );
   	if(sizeRead4 !=total){exit(-1);}
	

	//Se aplica la funcion de escala de grises y se guardan los valores en una matriz
	for(unsigned int i = 0; i< longitud; i++){
		gris[i] = (0.3*rojo[i] + 0.59*verde[i] + 0.11*azul[i]);
	}

	//Variables para los Tramos
	double bytesT = 256.0/(double)tramos;
	unsigned int arrayTramos [tramos];
	
	double bytesTAc = 0;
	double bytesTAcSig = bytesT;

	
	
    	FILE * escribir;

      	//Se abre el fichero
    	escribir = fopen(destino, "wb+");

  	//Control de error de fichero
	if (!escribir)
	{
	  exit(-1);
	}
	
	
	std::string s;
	//Bucle que recorre los grises por tramo y suma en la posicion del array correspondiente el numero total de valores en ese tramo
	for(int i=0; i< tramos; i++){
        	arrayTramos[i] = 0;
		
		for(unsigned int k = 0; k < longitud; k++){
			if( bytesTAc <= gris[k] && gris[k]<=(bytesTAcSig - 1)){
				arrayTramos [i] += 1;
			}
		}
		//Para cada tramo añadir el valor total del tramo a un string
		//En el ultimo tramo no se escribe el espacio al final
		if (i==(tramos-1)){
			s += std::to_string(arrayTramos[i]);
			
		}else{
			s += std::to_string(arrayTramos[i]);
			s+= " ";

		}
		//Se actualiza el valor de los limites superior e inferior de los tramos
		bytesTAc += bytesT;
		bytesTAcSig += bytesT; 	
	}
	
	//Escribir el valor en el fichero
	fprintf(escribir, "%s" , s.c_str());
	
        //Liberar espacio
	delete[] rojo;
	delete[] verde;
	delete[] azul;
	delete[] gris;
	    
	fclose(ifp);
	fclose(escribir);
}


void maximoMinimo(char *filename, char *destino){

	//Se declaran variables
	FILE * ifp;
	unsigned int altura, anchura;
	int maxR=0, minR=0, maxA=0, minA=0, maxV=0, minV=0; 

	//Se abre el fichero
   	ifp = fopen(filename, "rb");

	//Control de error de fichero
	if (!ifp){
    	exit(-1);
	}
	//Lectura de fichero
	size_t sizeRead0 = fread(&altura, sizeof(int), 1, ifp );
	if(sizeRead0 ==0){exit(-1);}
	size_t sizeRead1 = fread(&anchura, sizeof(int), 1, ifp );
	if(sizeRead1 ==0){exit(-1);}
	
	//Declaracion de variables para las matrices de colores
	unsigned int longitud = altura * anchura;
	unsigned char* rojo = new unsigned char[longitud];
	unsigned char* verde = new unsigned char[longitud]; 
	unsigned char* azul = new unsigned char[longitud]; 
	unsigned int total= (int)(longitud/sizeof(char));
	
	//Se leen los colores, guardando los valores en la matriz correspondiente
	size_t sizeRead2 = fread(&rojo[0], sizeof( char), total, ifp );
	if(sizeRead2 !=total){exit(-1);}
	size_t sizeRead3 = fread(&verde[0], sizeof(char), total, ifp );
	if(sizeRead3!=total){exit(-1);}
	size_t sizeRead4 = fread(&azul[0], sizeof(char), total, ifp );
	if(sizeRead4 !=total){exit(-1);}
	
	//Se hallan los valores de maximo y minimo para las matrices de colores
	for(unsigned int i = 0; i< longitud; i++){
		if((int)rojo[i] < minR){ minR = (int)rojo[i];}
		if((int)rojo[i] > maxR){ maxR = (int)rojo[i];}				
		if((int)verde[i] < minV){ minV = (int)verde[i];}
		if((int)verde[i] > maxV){ maxV = (int)verde[i];}
		if((int)azul[i] < minA){ minA = (int)azul[i];}
		if((int)azul[i] > maxA){ maxA = (int)azul[i];}
	}
	
    	FILE * escribir;

	//Se abre el fichero
 	escribir = fopen(destino, "wb+");
	//Control de error de fichero
	if (!escribir)
	{
	  exit(-1);
	}
	
	//Escribir el valor de maximos y minimos en el fichero
	fprintf(escribir, "%d%c%d%c%d%c%d%c%d%c%d" , maxR,espacio, minR,espacio, maxV,espacio, minV,espacio, maxA,espacio, minA);
	
	//Liberar memoria
	delete[] rojo;
   	delete[] verde;
    	delete[] azul;

    	fclose(ifp);
    	fclose(escribir);
}

void mascara(char *filename, char *rutaMascara, char *destino){

	//Se declaran variables
	unsigned int altura, anchura;
	
	FILE * ifp;

	//Se abre el fichero
   	ifp = fopen(filename, "rb");

	//Control de error de fichero
	if (!ifp)
	{
      	exit(-1);
	}

	//Lectura de fichero
    	size_t sizeRead0 = fread(&altura, sizeof(int), 1, ifp );
    	if(sizeRead0 ==0){exit(-1);}

    	size_t sizeRead1 = fread(&anchura, sizeof(int), 1, ifp );
    	if(sizeRead1 ==0){exit(-1);}

	//Declaracion de variables para las matrices de colores
	unsigned int longitud =altura*anchura;
	unsigned int total= (int)(longitud/sizeof(char));
	unsigned char* rojo = new unsigned char[longitud];
	unsigned char* verde = new unsigned char[longitud]; 
	unsigned char* azul = new unsigned char[longitud];
	
	//Se leen los colores, guardando los valores en la matriz correspondiente
	size_t sizeRead2 = fread(&rojo[0], sizeof( char), total, ifp );
	if(sizeRead2 !=total){exit(-1);}
	size_t sizeRead3 = fread(&verde[0], sizeof(char), total, ifp );
	if(sizeRead3!=total){exit(-1);}
	size_t sizeRead4 = fread(&azul[0], sizeof(char), total, ifp );
	if(sizeRead4 !=total){exit(-1);}
	
	//Se declaran variables
	unsigned int alturaM, anchuraM;
	FILE * ifpM;

	//Se abre el fichero
   	ifpM = fopen(rutaMascara, "rb");

	//Control de error de fichero
	if (!ifpM)
	{
		exit(-1);
	}
	//Lectura de fichero
   	size_t sizeRead5 = fread(&alturaM, sizeof(int), 1, ifpM );
    	if(sizeRead5 ==0){exit(-1);}

    	size_t sizeRead6 = fread(&anchuraM, sizeof(int), 1, ifpM );
    	if(sizeRead6 ==0){exit(-1);}

	//Si la alura o anchura de la imagen y la mascara no son iguales no aplica
	if(alturaM != altura && anchuraM != anchura){
		exit(-2);
	}else{		
		//Declaracion de variables para las matrices de colores
		unsigned char* rojoM = new unsigned char[longitud];
		unsigned char* verdeM = new unsigned char[longitud]; 
		unsigned char* azulM = new unsigned char[longitud];
	
		//Se leen los colores, guardando los valores en la matriz correspondiente	
		size_t sizeRead7 = fread(&rojoM[0], sizeof( char), total, ifpM );
		if(sizeRead7 !=total){exit(-1);}
		
		size_t sizeRead8 = fread(&verdeM[0], sizeof( char), total, ifpM );
		if(sizeRead8 !=total){exit(-1);}
		
		size_t sizeRead9 = fread(&azulM[0], sizeof( char), total, ifpM );
		if(sizeRead9 !=total){exit(-1);}
		
		//Se aplica la mascara
		for(unsigned int i = 0; i< longitud; i++){
		    	rojo[i] = rojo[i] * rojoM[i];
			verde[i] = verde[i] * verdeM[i];
			azul[i] = azul[i] * azulM[i];
		}	

	       	FILE * escribir;

		//Se abre el fichero
		escribir = fopen(destino, "wb+");

		//Control de error de fichero
		if (!escribir)
		{
			exit(-1);
		}
		//Se escribe en el fichero las matrices de color con la mascara aplicada
		fwrite (&altura , sizeof(unsigned int), 1, escribir);
		fwrite (&anchura , sizeof(unsigned int), 1, escribir);
		fwrite (rojo , sizeof(unsigned char), longitud, escribir);
		fwrite (verde , sizeof(unsigned char), longitud, escribir);
		fwrite (azul , sizeof(unsigned char), longitud, escribir);

		//Liberar memoria
		delete[] rojo;
		delete[] verde;
		delete[] azul;
		delete[] rojoM;
		delete[] verdeM;
		delete[] azulM;

		fclose(ifp);
		fclose(escribir);
	}
}

void rotacion(char *filename, double grados ,char *destino){

	//Se declaran variables
	unsigned int altura, anchura;
	FILE * ifp;

	//Se abre el fichero
   	ifp = fopen(filename, "rb");

	//Control de error de fichero
	if (!ifp)
	{
    	exit(-1);
	}
    
	//Lectura de fichero
    	size_t sizeRead0 = fread(&altura, sizeof(int), 1, ifp );
    	if(sizeRead0 ==0){exit(-1);}

   	size_t sizeRead1 = fread(&anchura, sizeof(int), 1, ifp );
    	if(sizeRead1 ==0){exit(-1);}

	//Declaracion de variables para las matrices de colores
	unsigned char* rojo=(unsigned char*)malloc(altura*anchura*sizeof(unsigned char));
	unsigned char* verde=(unsigned char*)malloc(altura*anchura*sizeof(unsigned char));
	unsigned char* azul=(unsigned char*)malloc(altura*anchura*sizeof(unsigned char));
	

	//Se leen los colores, guardando los valores en la matriz correspondiente	
	size_t sizeRead2 = fread(rojo, sizeof(unsigned char), anchura*altura, ifp );
 	if(sizeRead2 !=anchura*altura){exit(-1);}

	size_t sizeRead3 = fread(verde, sizeof(unsigned char), anchura*altura, ifp );
    	if(sizeRead3 !=anchura*altura){exit(-1);}

	size_t sizeRead4 = fread(azul, sizeof(unsigned char), anchura*altura, ifp );
        if(sizeRead4 !=anchura*altura){exit(-1);}
	
    	//Declaracion de variables para las matrices de colores rotadas
	unsigned char* rojoR=(unsigned char*) malloc(altura*anchura*sizeof(unsigned char));
	unsigned char* verdeR=(unsigned char*) malloc(altura*anchura*sizeof(unsigned char));
	unsigned char* azulR=(unsigned char*) malloc(altura*anchura*sizeof(unsigned char));


    	//Calculo de la rotacion aplicando las formulas necesarias    
	double gradosC = ((grados*M_PI)/180.0);
	double xCentro = (anchura/2.0);
	double yCentro = (altura/2.0);
	    
	double coseno = cos(gradosC);
	double seno = sin(gradosC);
	
	double xi = 0, yi = 0;
	unsigned int xf = 0, yf =  0;
    	
	//Bucle que recorre todas las posiciones de las matrices
	for(unsigned int i = 0; i< altura; i++){
		for(unsigned int j = 0; j< anchura; j++){
			
			//Posicion respecto del centro
			xi = i - xCentro;
			yi = j - yCentro;
            
            		xf = ceil((coseno*xi) - (seno*yi) + xCentro);
            		yf = ceil((seno*xi) + (coseno*yi) + yCentro);
            		
			//Si la posicion entra en el rango se aplica la rotacion
            		if(xf >= 0 && xf < anchura && yf >= 0 && yf < altura ){	
            			rojoR[((yf*anchura)+xf)] = rojo[((j*anchura)+i)];
				verdeR[((yf*anchura)+xf)] = verde[((j*anchura)+i)];
				azulR[((yf*anchura)+xf)] = azul[((j*anchura)+i)];		          
            		}			
 		}	
	}
	

	FILE * escribir;

	//Se abre el fichero
	escribir = fopen(destino, "wb+");

	//Control de error de fichero
	if (!escribir)
	{
		exit(-1);
	}
	
	//Se escribe en el fichero las matrices de colores rotadas
	fwrite (&altura , sizeof(int), 1, escribir);
	fwrite (&anchura , sizeof(int), 1, escribir);

	for(unsigned int i = 0; i< altura*anchura; i++){		
		fwrite (&rojoR[i] , sizeof(unsigned char), 1, escribir);
	}
	
	for(unsigned int i = 0; i< altura*anchura; i++){	
		fwrite (&verdeR[i] , sizeof(unsigned char), 1, escribir);
	}

	for(unsigned int i = 0; i< altura*anchura; i++){			
		fwrite (&azulR[i] , sizeof(unsigned char), 1, escribir);
	}	
			
	//Cerrar descriptores		
	fclose(ifp);
	fclose(escribir);
}


void filtro(char *filename, double radio ,char *destino){

	//Se declaran variables
	unsigned int altura, anchura;
	FILE * ifp;

	//Se abre el fichero
   	ifp = fopen(filename, "rb");

	//Control de error de fichero
	if (!ifp)
	{
		exit(1);
	}

	//Lectura de fichero
    	size_t sizeRead0 = fread(&altura, sizeof(int), 1, ifp );
    	if(sizeRead0 ==0){exit(-1);}

    	size_t sizeRead1 = fread(&anchura, sizeof(int), 1, ifp );
    	if(sizeRead1 ==0){exit(-1);}

	//Declaracion de variables para las matrices de colores
	unsigned char** rojo=new unsigned char*[anchura];
	for (unsigned int i = 0; i < anchura; i++){
		rojo[i]= new unsigned char[altura];
	}

	unsigned char** verde=new unsigned char*[anchura];
	for (unsigned int i = 0; i < anchura; i++){
		verde[i]= new unsigned char[altura];
	}
	
	unsigned char** azul=new unsigned char*[anchura];
	for (unsigned int i = 0; i < anchura; i++){
		azul[i]= new unsigned char[altura];
	}

	//Se leen los colores mediante un bucle, guardando los valores en la matriz correspondiente
	unsigned int total= (int)(anchura/sizeof(char));
	for(unsigned int i = 0; i< altura; i++){
            size_t sizeRead2 = fread(&rojo[i][0], sizeof(char), total, ifp );
            if(sizeRead2 !=total){exit(-1);}
 			
	}
		
	
	for(unsigned int i = 0; i< altura; i++){
            size_t sizeRead3 = fread(&verde[i][0], sizeof(char), total, ifp );
            if(sizeRead3 !=total){exit(-1);}
 			
	}

	for(unsigned int i = 0; i< altura; i++){
            size_t sizeRead4 = fread(&azul[i][0], sizeof(char), total, ifp );
            if(sizeRead4 !=total){exit(-1);}
 		
	}

   	//Se aplican las formulas necesarias para aplicar el filtro
	double xf =0.0,yf=0.0;
	double powradio=pow(radio,2.0);
	for(unsigned int i = 0; i< altura; i++){
		for(unsigned int j = 0; j< anchura; j++){

			xf =0.0,yf=0.0;

            		xf = i -(anchura/2.0);
            		yf = j -(altura/2.0);
            
            		//Se aplica el filtro a todo lo que cumpla la ecuacion
            		if(((xf*xf)+(yf*yf))>powradio){

				rojo[j][i] = floor(0.3*((double)rojo[j][i]));
				verde[j][i] =floor(0.59*((double)verde[j][i]));
				azul[j][i] = floor(0.11*((double)azul[j][i]));

           		 }
 		}	
	}
	
	
	//Descriptor de fichero	
	FILE * escribir;

	//Se abre el fichero
	escribir = fopen(destino, "wb+");

	//Control de error de fichero
	if (!escribir)
	{
		exit(-1);
	}

	//Se escribe en el fichero las matrices de colores rotadas
	fwrite (&altura , sizeof(int), 1, escribir);
	fwrite (&anchura , sizeof(int), 1, escribir);
	

	for(unsigned int i = 0; i< altura; i++){
	  	
	      	fwrite (&rojo[i][0] , sizeof(unsigned char), total, escribir);
	  	
	}
	for(unsigned int i = 0; i< altura; i++){
	  	
		fwrite (&verde[i][0] , sizeof(unsigned char), total, escribir);
		
	}
	for(unsigned int i = 0; i< altura; i++){
		
		fwrite (&azul[i][0] , sizeof(unsigned char), total, escribir);
		
	}

	//Liberar memoria
	delete[] rojo;
	delete[] verde;
	delete[] azul;		

	fclose(ifp);
	fclose(escribir);

}


int main(int argc, char *argv[]){
	
    	char *rutaI=NULL;
   	char *rutaO=NULL;
    
	int uso = 0, nTramos = 0;
	char *rutaM=NULL;
	double  nGrados = 0, radio = 0;
	//Bucle que recorre todos los argumentos
	for(int k=0;k<argc;k++){
        	//Si se encuentra "-u"
		if(strcmp(argv[k],"-u")==0){
			uso = atoi(argv[k+1]);
			//Si el valor que acompaña a "-u" es 0 Se ejecutara la funcion histograma
			if(uso==0){
				for(int j=0;j<argc;j++){     
					for(int i=0;i<argc;i++){   
						//Se obtienen las direccines de entrada y salida                        
						if(strcmp(argv[i],"-i")==0){
						       	rutaI=argv[i+1];
						}
						
						if(strcmp(argv[i],"-o")==0){
						      	rutaO=argv[i+1];
						}
                        		}
					//Se obtiene el numero de tramos 
					if(strcmp(argv[j],"-t")==0){
						nTramos = atoi(argv[j+1]); 
                        			histograma(rutaI, nTramos, rutaO);
					}
                		}
			}
			//Si el valor que acompaña a "-u" es 1 Se ejecutara la funcion maximoMinimo
            		if(uso==1){                
                		for(int i=0;i<argc;i++){  
					//Se obtienen las direccines de entrada y salida                           
                    			if(strcmp(argv[i],"-i")==0){
                        			rutaI=argv[i+1];
                    			}                    
                    			if(strcmp(argv[i],"-o")==0){
                        			rutaO=argv[i+1];
                    			}
               			}     
				maximoMinimo(rutaI, rutaO);
			}
			//Si el valor que acompaña a "-u" es 2 Se ejecutara la funcion mascara
			if(uso==2){
				for(int j=0;j<argc;j++){                    
                    			for(int i=0;i<argc;i++){   
						//Se obtienen las direccines de entrada y salida                              
                        			if(strcmp(argv[i],"-i")==0){
                            				rutaI=argv[i+1];
                        			}                        
                        			if(strcmp(argv[i],"-o")==0){
                            				rutaO=argv[i+1];
                        			}
                    			}                    
					//Se obtiene la ruta de la mascara
					if(strcmp(argv[j],"-f")==0){
						rutaM=argv[j+1];
						mascara(rutaI, rutaM,rutaO);                        
					}
                		}             
			}
			//Si el valor que acompaña a "-u" es 3 Se ejecutara la funcion rotacion 
			if(uso==3){
				for(int j=0;j<argc;j++){                    
		                    	for(int i=0;i<argc;i++){  
						//Se obtienen las direccines de entrada y salida                               
                        			if(strcmp(argv[i],"-i")==0){
                            				rutaI=argv[i+1];
                        			}                        
                        			if(strcmp(argv[i],"-o")==0){
                            				rutaO=argv[i+1];
                        			}
                   			}  
					//se obtienen los grqados con los que se rota la imagen                  
					if(strcmp(argv[j],"-a")==0){
						nGrados = atoi(argv[j+1]);
						rotacion(rutaI, nGrados,rutaO);                                                
					}
                		}               
			}
			//Si el valor que acompaña a "-u" es 4 Se ejecutara la funcion filtro
			if(uso==4){
				for(int j=0;j<argc;j++){                    
                    			for(int i=0;i<argc;i++){        
						//Se obtienen las direccines de entrada y salida                         
                        			if(strcmp(argv[i],"-i")==0){
                            				rutaI=argv[i+1];
                        			}                        
                        			if(strcmp(argv[i],"-o")==0){
                            				rutaO=argv[i+1];
                        			}
                    			}   
					//Se obtiene el radio con el que se aplica el filtro                 
					if(strcmp(argv[j],"-r")==0){
						radio = atoi(argv[j+1]);
						filtro(rutaI, radio,rutaO);                        
					}
				}                
			}
		}
	}
	return 0;
}
