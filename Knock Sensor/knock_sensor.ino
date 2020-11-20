/*
    original project: http://grathio.com/2009/11/secret_knock_detecting_door_lock/
*/


const int _SENSIBILIDADE = 500;  /* Sensor sensibility */
const int _ESPERA_FIM = 1220;    /* Defines idle time after knock sequence */
const int _TAMANHO_MATRIZ = 15;  /* Max matrix size. Accepts 15 knock positions */
const int _TOLERANCIA_ERRO = 35; /* Error range, +- 1ms */
const int _BTN_GRAVAR = 8;       /* Button pin placement */

//int matrizSequenciaGravada[_TAMANHO_MATRIZ] = {0,126,120,134,186,463,196,0,0,0};
int matrizSequenciaGravada[_TAMANHO_MATRIZ] = {251,86,80,187,437,212}; /* Initial sequence */

int matrizSequencia[_TAMANHO_MATRIZ]; /* Defines matrix to receive user sequence */
int indiceCorrente = 0; /* Stars current to receive user sequence */

void setup() {
  pinMode(A0,INPUT);
  pinMode(_BTN_GRAVAR,INPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
}

/* Resets user matrix "A0" */
void inicializaMatriz(){
  indiceCorrente = 0;
  for(int i=0;i<_TAMANHO_MATRIZ;i++){
    matrizSequencia[i] = 0;
  }
}

void loop() {

  if( sensorAtivo() ) {
    inicializaMatriz();
    analisaSequencia();
  }

}

/* Verifies sensor activity */
boolean sensorAtivo(){
  return analogRead(A0)<_SENSIBILIDADE?true:false;
}

/* Starts sequence analysis */
void analisaSequencia(){

  int inicio = millis();
  int agora = millis();
  boolean consulta = true; 
  delay(60);
  /* Waits 6 seconds between sensor verification  */
  /* Executes loop */
  do {
    agora = millis();
    if( agora-inicio > _ESPERA_FIM ){
      consulta = false;
    }
  } while ( sensorAtivo() == false && consulta == true );


  if((agora-inicio)>_ESPERA_FIM){
    /* If time is bigger than idle, validates */
    validaSequencia();
  }else{
    /*
      Else adds to matrix 
    */
    matrizSequencia[indiceCorrente] = agora-inicio;
    indiceCorrente++;
    delay(60);
    analisaSequencia(); 
  }
}

/* Verifies if recording sequence button is pressed */
boolean gravarSequencia(){
  return digitalRead(_BTN_GRAVAR);
}

void validaSequencia(){
  boolean seqValida = true;

  /* Register knocks if button is pressed */
  if( gravarSequencia() ) {
    Serial.println( "Gravando sequencia!" );
    for(int i=0;i<_TAMANHO_MATRIZ;i++){
      matrizSequenciaGravada[i] = matrizSequencia[i];
      Serial.println( matrizSequencia[i] );
    }
    sequenciaGravada();
    Serial.println( "Sequencia gravada!" );
  }else{
    /* Validates if positions are within error range */
    Serial.println( "Inicio da sequencia" );
    for(int i=0;i<_TAMANHO_MATRIZ;i++){
      Serial.print( matrizSequencia[i] );
      Serial.print(" - ");
      Serial.print( matrizSequenciaGravada[i] );
      Serial.println( " " );
      if( matrizSequencia[i] > matrizSequenciaGravada[i]+_TOLERANCIA_ERRO || matrizSequencia[i] < matrizSequenciaGravada[i]-_TOLERANCIA_ERRO ) {
        /* If not, sequence invalid */
        seqValida = false;
      }
    }
    seqValida?sequenciaValida():sequenciaInvalida();
    Serial.println( "Fim da sequencia" );
  }
}

/* If invalid, lights the red LED and buzz on position 9 */
void sequenciaInvalida(){
  Serial.println( "Inválida" );
  digitalWrite(11,HIGH);
  tone(9,1000,1000);
  delay(1000);
  noTone(9);
  digitalWrite(11,LOW);
}

/* If invalid, lights the green LED and buzz on position 9 */
void sequenciaValida(){
  Serial.println( "Válida" );
  digitalWrite(10,HIGH);
  tone(9,2000,500);
  delay(150);
  tone(9,1500,500);
  delay(150);
  tone(9,2000,500);
  noTone(9);
  digitalWrite(10,LOW);
}

/* If recording sequence is validated, blink green */
void sequenciaGravada(){
  digitalWrite(10,HIGH);
  delay(1500);
  digitalWrite(10,LOW);
}

