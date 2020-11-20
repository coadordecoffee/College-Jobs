/*
    projeto original: http://grathio.com/2009/11/secret_knock_detecting_door_lock/
*/


const int _SENSIBILIDADE = 500;  /* Define a sensibilidade do sensor */
const int _ESPERA_FIM = 1220;    /* Define quanto tempo ocioso o sistema entende que termina a sequencia */
const int _TAMANHO_MATRIZ = 15;  /* Define o tamanho máximo da matriz. A sequencia será lida até apenas as 15 primeiras posições */
const int _TOLERANCIA_ERRO = 35; /* Define um range de tolerancia a erro, para mais e para menos em milisegundos */
const int _BTN_GRAVAR = 8;       /* Constante que define o pino onde está situado o botão, utilizado como flag para programar a sequecia */

//int matrizSequenciaGravada[_TAMANHO_MATRIZ] = {0,126,120,134,186,463,196,0,0,0};
int matrizSequenciaGravada[_TAMANHO_MATRIZ] = {251,86,80,187,437,212}; /* Sequencia inicial - Esta é carregada até mesmo após desligado o Arduino */

int matrizSequencia[_TAMANHO_MATRIZ]; /* Define a matriz que receberá a sequencia do usuário, pra depois comparar com a matriz "matrizSequenciaGravada" */
int indiceCorrente = 0; /* Inicializa variável utilizada para gravar o espaço de tempo na posição incrementada por "indiceCorrente" */

void setup() {
  pinMode(A0,INPUT);
  pinMode(_BTN_GRAVAR,INPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
}

/* Função utilizada para zerar os valores dentro da matriz que irá guardar os ruidos do leitor "A0" */
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

/* Verifica se o sensor está ativo, onde a leitura analogica deve ser < que a sensibilidade definida em _SENSIBILIDADE */
boolean sensorAtivo(){
  return analogRead(A0)<_SENSIBILIDADE?true:false;
}

/* Inicia a análise da sequencia, quando lê a primeira ativação do sensor, entra em um laço até que o sensor fique em repouso 1200 milisegundos (1,2 segundo) */
void analisaSequencia(){

  int inicio = millis();
  int agora = millis();
  boolean consulta = true; /* Variável que controla a iteração do laço */
  delay(60);
  /*
    Aguarda 60 milisegundos para realizar nova verificação se o sensor está ativo.
    Este tempo é importante, se ele não existir, ao passar pelo loop o sensor ainda estará ativo
  */

  /* Executa o laço pelo menos uma vez */
  do {
    agora = millis();
    if( agora-inicio > _ESPERA_FIM ){ /* Verifica se o tempo que o leitor ficou sem receber sinal é maior que o valor definido por "_ESPERA_FIM" (originalmente 1,2 segundo) */
      consulta = false;
    }
  } while ( sensorAtivo() == false && consulta == true );


  if((agora-inicio)>_ESPERA_FIM){
    /* Se o tempo de espera é maior que "_ESPERA_FIM" valida a sequencia informada */
    validaSequencia();
  }else{
    /*
      Senão incrementa o indice da matriz, e aloca o valor do tempo inicial em que o sensor está ativo.
      até o próximo momento que fica ativo novamente, este tempo sempre será menor ou igual a "_ESPERA_FIM"
    */
    matrizSequencia[indiceCorrente] = agora-inicio;
    indiceCorrente++;
    delay(60);
    analisaSequencia(); /* Novamente executa a escuta no sensor e aloca nova posição */
  }
}

/* Verifica se o botão que define a gravação está pressionado */
boolean gravarSequencia(){
  return digitalRead(_BTN_GRAVAR);
}

void validaSequencia(){
  boolean seqValida = true;

  /* Se o botão gravarSequencia está pressionado grava na matriz "matrizSequenciaGravada" o valor contido na matriz informada pelo usuário */
  if( gravarSequencia() ) {
    Serial.println( "Gravando sequencia!" );
    for(int i=0;i<_TAMANHO_MATRIZ;i++){
      matrizSequenciaGravada[i] = matrizSequencia[i];
      Serial.println( matrizSequencia[i] );
    }
    sequenciaGravada();
    Serial.println( "Sequencia gravada!" );
  }else{
    /* Valida se todas as posições da matriz se apresentam no range lido entre (+-) o valor de "_TOLERANCIA_ERRO" */
    Serial.println( "Inicio da sequencia" );
    for(int i=0;i<_TAMANHO_MATRIZ;i++){
      Serial.print( matrizSequencia[i] );
      Serial.print(" - ");
      Serial.print( matrizSequenciaGravada[i] );
      Serial.println( " " );
      if( matrizSequencia[i] > matrizSequenciaGravada[i]+_TOLERANCIA_ERRO || matrizSequencia[i] < matrizSequenciaGravada[i]-_TOLERANCIA_ERRO ) {
        /* Se qualquer posição não estiver no range, retorna como inválida a sequencia */
        seqValida = false;
      }
    }
    seqValida?sequenciaValida():sequenciaInvalida();
    Serial.println( "Fim da sequencia" );
  }
}

/* Executa ações se a sequencia é inválida. Neste caso acende um led vermelho no pino 11 e aciona um buzzer no pino 9 */
void sequenciaInvalida(){
  Serial.println( "Inválida" );
  digitalWrite(11,HIGH);
  tone(9,1000,1000);
  delay(1000);
  noTone(9);
  digitalWrite(11,LOW);
}

/* Executa ações se a sequencia é válida. Neste caso acende um led verde no pino 11 e aciona um buzzer no pino 9 */
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

/* Realiza ações quando a sequencia é gravada com sucesso, acendendo o led verde no pino 10 por 1,5 segundo */
void sequenciaGravada(){
  digitalWrite(10,HIGH);
  delay(1500);
  digitalWrite(10,LOW);
}

