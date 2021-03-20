#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <string.h>
#include <math.h>

//prioridad=3: ^ ; prioridad=2: * / ; prioridad=1: + - ; prioridad=0: ( ; 

//estructura para la lista de polaca inversa
typedef struct data {
	char *valor;//valor que puede ser el operador, numero o variable
	int isOperador;//1: es operador ; 0: no es operador y es un valor o variable
	struct data *siguiente;//para el dato siguiente
}ListTokens;

typedef struct pila {
	char operador;
	int procedencia;//Nivel de prioridad del operador
	struct pila *siguiente;
}Pila;

ListTokens *tokens=NULL;

ListTokens *agregarToken(char *valor,int isOperador){//lista enlazada en polaco inversa
	//Aquí, para un mejor manejo para convertir en arbol binario, los argumentos que se ingresan siempre seran los primeros en la lista
	ListTokens *newToken;
	newToken=(ListTokens *)malloc(sizeof(ListTokens));//asigno un almacenamiento dinámico

	//la variable valor debe ser almacenado en un espacio de memoria diferente, ya que *valor hace referencia a mi array 'num', y 'num' cambia
	int len = strlen(valor);   
	char *valorTemp = calloc(1, len + 1);  
	memcpy(valorTemp,valor,len);

	newToken->valor=valorTemp;//
	newToken->isOperador=isOperador;
	newToken->siguiente=NULL;

	if (tokens!=NULL){
		newToken->siguiente=tokens;
	}

	return newToken;
}

Pila *agregarPila(Pila *pila,char operador, int procedencia){//referencias para en polaco inversa
	//Aquí, para un mejor manejo para eliminar y obtener los operadores, los argumentos que se ingresan siempre seran los primeros en la lista
	Pila *newPila;
	newPila=(Pila *)malloc(sizeof(Pila));
	newPila->operador=operador;
	newPila->procedencia=procedencia;
	newPila->siguiente=NULL;
	if (pila!=NULL){
		newPila->siguiente=pila;
	}
	return newPila;
}

Pila *isAgregarToken(Pila *pila,char operador, int procedencia){//para saber cuando agregar los operadores de la pila en la lista

	void *setPilaString(Pila *);//difino la funcion
	Pila *removePilaParentesis(Pila *,int);//difino la funcion

	if (pila!=NULL){//evaluo si es nulo,
		if (operador!='('){//evaluo si es el operador es '(', porque su prioridad no es relevante
			if (operador==')'){//evaluo si es el operador es ')', para poder agregar los operadores hasta encontra en primer '('
				//Eliminar hasta el primer encuentro de (
				pila=removePilaParentesis(pila,0);
				
			}else{
				if ((pila->operador!='(')){
					if(procedencia<=pila->procedencia){//aqui la pila se convierte en null, y agregamos los operadores de la pila a la lista
						setPilaString(pila);
						pila=NULL;
						//printf("\nProbando-----------------> %s",cadena);
						//printf("\n-------------El operador %c es <= que %c",operador,pila->operador);
					}
				}
			}
		}
	}

	return pila;

}

//funcion para ingresar los operadores dentro de los parentesis a la lista de polaca inversa
Pila *removePilaParentesis(Pila *pila,int encontrado){// encontrado==> 0: primera entrada; 1: buscando; 2: encontrado

	if((pila->operador=='(')&&encontrado==0){
		return pila->siguiente;
	}

	if (pila->operador!='('){
		if (encontrado==2){
			return pila;
		}else{
			char dataTemp[10] = "";
			dataTemp[0]=pila->operador;
			tokens=agregarToken(dataTemp,1);
			if (encontrado==0){
				pila=removePilaParentesis(pila->siguiente,1);
			}else{
				pila=removePilaParentesis(pila->siguiente,encontrado);
			}
		}
	}else{
		if (pila->siguiente==NULL){
			return NULL;
		}
		pila=removePilaParentesis(pila->siguiente,2);
	}
	
	
	return pila;

}

//funcion para ingresar los operadores
void *setPilaString(Pila *pila){

	char dataTemp[10] = "";

	if (pila!=NULL){
		if (pila->operador!='('){
			dataTemp[0]=pila->operador;
			tokens=agregarToken(dataTemp,1);
			setPilaString(pila->siguiente);
			/*char *cadena=setPilaString(tokens,pila->siguiente);
			strcat(dataTemp,cadena);
			free(cadena);*/
		}
	}

	/*int len = strlen(dataTemp);   
    char *data = calloc(1, len+1);  
	memcpy(data,dataTemp,len);
	
	return data;*/

}

typedef struct nodo {
	char *valorText;
	int valor;
	struct nodo *izq;
	struct nodo *der;
	int operacion;//para saber si hace una operacion=> 0: solo es un valor o variable; 1: realiza una operacion el nodo 
}Arbol;

struct nodo *raiz=NULL;

Arbol *convertirArbol(ListTokens *tokenPre) {//Para establecer el nodo principal o primer nodo en crear apartir de la lista de polaca inversa
	
	ListTokens *agregarNodo(Arbol *,ListTokens *);

	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	nuevo->valorText=NULL;
	nuevo->valor=0;
	nuevo->operacion=0;
	nuevo->izq=NULL;
	nuevo->der=NULL;

	//para iniciar el primer nodo
	if (tokenPre->isOperador==1){//si es operador, tiene nodos izquierdos y derechos
		nuevo->valorText=tokenPre->valor;//almaceno el operador
		nuevo->operacion=1;
		//la función agregarNodo devuelve la ultima referencia de la lista de polaca inversa
		ListTokens *tokenRes=agregarNodo(nuevo,tokenPre->siguiente);//primero agrego el nodo derecho
		agregarNodo(nuevo,tokenRes->siguiente);//luego insertdo el nodo izquierdo
	}else{//si es x o un numero, no tiene nodos izquierdos ni derechos
		if (strcmp(tokenPre->valor,"x") == 0){
			nuevo->valorText="x";
		}else{
			int num=atoi(tokenPre->valor);
			nuevo->valor=num;
		}
	}
	raiz=nuevo;
}

struct nodo *nodoTemp=NULL;

ListTokens *agregarNodo(Arbol *nodoPre,ListTokens *tokenPre) {// para agregar un nodo, en los argumentos recibo el nodo actual

	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	nuevo->valorText=NULL;
	nuevo->valor=0;
	nuevo->operacion=0;
	nuevo->izq=NULL;
	nuevo->der=NULL;

	if (tokenPre->isOperador==1){//si es operador, tiene nodos izquierdos y derechos
		nuevo->valorText=tokenPre->valor;
		nuevo->operacion=1;
		ListTokens *tokenDer=agregarNodo(nuevo,tokenPre->siguiente);//primero agrego el nodo derecho
		ListTokens *tokenIzq=agregarNodo(nuevo,tokenDer->siguiente);//luego insertdo el nodo izquierdo
		if (nodoPre->der==NULL){//reviso si el nodo actual, tiene un nodo a la derecho
			nodoPre->der=nuevo;//agrego a la derecha del nodo actual, el nuevo sigueinte
		}else{
			nodoPre->izq=nuevo;//agrego a la izquierda del nodo actual, el nuevo sigueinte
		}
		return tokenIzq;
	}else{// si es un numero o variable, no tiene nodos derechos ni izquierdos
		if (strcmp(tokenPre->valor,"x") == 0){
			nuevo->valorText="x";//agrego la varialbe x
		}else{
			int num=atoi(tokenPre->valor);
			nuevo->valor=num;//agrego el numero
		}
		if (nodoPre->der==NULL){//reviso si el nodo actual, tiene un nodo a la derecho
			nodoPre->der=nuevo;//agrego a la derecha del nodo actual, el nuevo sigueinte
		}else{
			nodoPre->izq=nuevo;//agrego a la izquierda del nodo actual, el nuevo sigueinte
		}
		return tokenPre;
	}

}

float recorridoPost(Arbol *reco,float x) {// recorrido Post Odern para realizar el cálculo
	if (reco != NULL) {
		if (reco->operacion==0){//operacion es 0 cuando es un numero o variable 'x'
			if (reco->valorText!=NULL){//si texto es diferente de nulo, entonces es la variable 'x'
				//printf("\n%s",reco->valorText);
				return x;
			}else{
				//printf("\n%d",reco->valor);
				return (float)reco->valor;
			}
		}else{
			float a=recorridoPost(reco->izq,x);
			float b=recorridoPost(reco->der,x);
			if (strcmp(reco->valorText,"+") == 0){
				float valorTemp=a+b;
				printf("\nOperacion: %4.2f + %4.2f = %4.2f",a,b,valorTemp);
				return a+b;
			}else{
				if (strcmp(reco->valorText,"-") == 0){
					float valorTemp=a-b;
					printf("\nOperacion: %4.2f - %4.2f = %4.2f",a,b,valorTemp);
					return a-b;
				}else{
					if (strcmp(reco->valorText,"/") == 0){
						float valorTemp=a/b;
						printf("\nOperacion: %4.2f / %4.2f = %4.2f",a,b,valorTemp);
						return a/b;
					}else{
						if (strcmp(reco->valorText,"*") == 0){
							float valorTemp=a*b;
							printf("\nOperacion: %4.2f * %4.2f = %4.2f",a,b,valorTemp);
							return a*b;
						}else{
							float valorTemp=pow(a,b);
							printf("\nOperacion: %4.2f ^ %4.2f = %4.2f",a,b,valorTemp);
							return pow(a,b);
						}
					}
				}
			}
		}
	}
}

void recorrer(Arbol *reco){//ayuda para recorrer el arbol en postOrden
	if (reco != NULL) {
		if (reco->operacion==0){
			if (reco->valorText!=NULL){
				printf("\nVariable: %s",reco->valorText);
			}else{
				printf("\nNumber: %d",reco->valor);
			}
		}else{
			recorrer(reco->izq);//4*x^3+x^2-5*x-2
			printf("\nOperador Matematico: %s",reco->valorText);
			recorrer(reco->der);
			printf("\nValor o: %d",reco->operacion);
		}
	}
}

void imprimirPre(struct nodo *reco){
    if (reco != NULL){
		if (reco->operacion==0){
			if (reco->valorText!=NULL){
        		printf(" %s",reco->valorText);
			}else{
        		printf(" %d",reco->valor);
			}
		}else{
        	printf(" %s",reco->valorText);
		}
        imprimirPre(reco->izq);
        imprimirPre(reco->der);
    }
}


void imprimirIn(struct nodo *reco){
    if (reco != NULL){
        imprimirIn(reco->izq);
		if (reco->operacion==0){
			if (reco->valorText!=NULL){
        		printf(" %s",reco->valorText);
			}else{
        		printf(" %d",reco->valor);
			}
		}else{
        	printf(" %s",reco->valorText);
		}
        imprimirIn(reco->der);
    }
}

void imprimirPost(struct nodo *reco){
    if (reco != NULL){
        imprimirPost(reco->izq);
        imprimirPost(reco->der);
		if (reco->operacion==0){
			if (reco->valorText!=NULL){
        		printf(" %s",reco->valorText);
			}else{
        		printf(" %d",reco->valor);
			}
		}else{
        	printf(" %s",reco->valorText);
		}
    }
}

void imprimirTokens(ListTokens *token) {
	if (token!= NULL) {
		printf("\n");
		printf("Termino: %s",token->valor);
		imprimirTokens(token->siguiente);
	}
}

void imprimirPila(Pila *pila) {//ayuda para polaca inversa
	if (pila!= NULL) {
		printf("\n");
		printf("Operador: %c",pila->operador);
		imprimirPila(pila->siguiente);
	}
}

int main() {
	
	char *cadena = (char *)malloc(100);//establezco memoria dinámica de longitud 100 bytes(100 caracteres)
	printf("Cadena: ");
	scanf("%s", cadena);//para escanear la cadena

	float numero;
	printf("Ingrese x: ");
	scanf("%f",&numero);//para escanear el valor de 'x'

	Pila *pila=NULL;//creo un puntero para almacenar la pila(se guardan los operadores) de polaca inversa

	char num[10] = "";//para poner numeros mayores de un digito
	int indicador=0;//para saber en que posicion de mi array 'num' va cada numero

	int isOperador(char letra);//declaro la funcion isOperador
	
	int len = strlen(cadena);
	int i;
	for (i = 0; i < len ; i++){//empezamos a recorrer la cadena ingresa para converitr en polaca inversa
		int resOperador=isOperador(cadena[i]);//evaluo si es un operador o no; 0: es un numero o variable ; >0: es un operador
		if (resOperador==0){
			num[indicador]=cadena[i];//almaceno el valor del caracter en mi array 'num'
			indicador=indicador+1;// aumento el indicador
			if (i==(len-1)){// evaluo si llegó al final de la cadena
				tokens=agregarToken(num,0);//agrego el ultimo numero de la cadena en la lista de polaca inversa
				setPilaString(pila);//agrela los valores de la pila en la lista de polaca inversa
			}
			/*if (cadena[i]=='x'){
				printf("\nLa variable es: %c",cadena[i]);
			}else{
				printf("\nEl numero es: %c",cadena[i]);
			}*/
		}else{
			if (indicador>0){//si el indicador es mayoy a 0 es porque mi array 'num' contiene un numero que debe guardarse en la lista de polaca inversa
				tokens=agregarToken(num,0);//agrego el numero de la cadena en la lista de polaca inversa
				memset(num,0,10);//vacio el array 'num'
				indicador=0;//reinicio el indicador a cero
			}
			pila=isAgregarToken(pila,cadena[i],resOperador);//para preguntar si debo eliminar operadores de la pila y agregar en la lista de polaca inversa, retorna la pila nueva
			if (resOperador!=5){//el 5 es ')', y este operador no debe agregarse en la pila, aquí se 
				pila=agregarPila(pila,cadena[i],resOperador);//para agregar el operador en la pila, retorna la pila nueva
			}else{
				if (i==(len-1)){//en cualquier caso, que al final de la cadena hay ')', debo agregar los ultimos operadores de la pila en la lista de polaca inversa
					setPilaString(pila);
				}
			}
			//printf("\nEl operador es: %c",cadena[i]);
		}
	}
	//imprimirTokens(tokens);
	//imprimirPila(pila);
	raiz=convertirArbol(tokens);//con esta funcion, empiezo a convertir la lista de polaca inversa a un arbol binario de expresión, retorna el arbol ya elaborado
    printf("\n\n");
	printf("Impresion preorden: ");
    imprimirPre(raiz);
    printf("\n\n");
    printf("Impresion Inorden: ");
    imprimirIn(raiz);
    printf("\n\n");
    printf("Impresion postorden: ");
    imprimirPost(raiz);
    printf("\n\n");
	float valor=recorridoPost(raiz,numero);//para realizar el calculo en PostOrden, devuelve el resultado
	printf("\nEl resultado es: %4.2f",valor);

	//(7+3/2)*x^3+x^2-5*x-2     4*x^3+x^2-5*x-2  (4/2)*x^2+x-4   x^2+(6/2)*x-4   x+(8/4)   x+(8/4)*x^3+5^2   (2+(7-3/2)*2-5)+x  x^2+(6/2)*x-4  (x+(3-4)/2)*x^2+(2-5/4)*x-5^2
	return 0;
}

int isOperador(char letra){//return=3: ^ ; return=2: * / ; return=1: + - ; return=4: ( ; return=5: ) ; 0 en caso de numero o variable
	
	if (letra=='+'){
		return 1;
	}else{
		if (letra=='-'){
			return 1;
		}else{
			if (letra=='*'){
				return 2;
			}else{
				if (letra=='/'){
					return 2;
				}else{
					if (letra=='^'){
						return 3;
					}else{
						if (letra=='('){
							return 4;
						}else{
							if (letra==')'){
								return 5;
							}else{
								return 0;
							}
						}
					}
				}
			}
		}
	}
}
