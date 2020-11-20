# Leitura da base de dados
base = read.csv('credit_data.csv')

# Apaga a coluna clientid
base$clientid = NULL

# Valores inconsistentes
base$age = ifelse(base$age < 0, 40.92, base$age)

# Valores faltantes
base$age = ifelse(is.na(base$age), mean(base$age, na.rm = TRUE), base$age)

# Escalonamento
base[, 1:3] = scale(base[, 1:3])

# Divis??o entre treinamento e teste
library(caTools)
set.seed(1)
divisao = sample.split(base$income, SplitRatio = 0.75)
base_treinamento = subset(base, divisao == TRUE)
base_teste = subset(base, divisao == FALSE)

# instala????o do pacote h2o para trabalhar com redes neurais
# install.packages('h2o')
library(h2o)

# m??todo para realizar a conex??o com o servidor dedicado
# vamos usar o argumento nthreads N??mero de threads no conjunto de encadeamentos. 
# Isso se relaciona ao n??mero de CPUs usadas. -1 significa usar todas as CPUs no host(padr??o). 
# Um inteiro positivo especifica o n??mero de CPUs diretamente. 
# Este valor ?? usado somente quando R inicia H2O.
h2o.init(nthreads = -1)

# vamos usar o m??todo h2o.deeplearning. O primeiro par??metro ?? o y, que vai receber 
# a vari??vel resposta (classe) da base (default). O par??metro ?? a base de treinamento
# como ela est?? no padr??o do R, precisamos transform??-la no padr??o h2o (as.h2o)
# pr??ximo argumento ?? a fun????o de ativa????o, aqui vamos usar a reLU
# hidden (camada escondida) colocamos em formato de vetor quantas camadas queremos
# se colocarmos o valor 100, significa que termos uma camada oculta com 100 neur??nios
# se colocarmos (100, 100) significa que termos duas camadas ocultas e cada uma ter?? 100 neur??nios
# epochs (??pocas) que ?? o training time, isto ?? quantas vezes vc vai fazer o ajuste de pesos
classificador = h2o.deeplearning(y = 'default',
                                 training_frame = as.h2o(base_treinamento),
                                 activation = 'Rectifier',
                                 hidden = c(100, 150),
                                 epochs = 1000)
previsoes = h2o.predict(classificador, newdata = as.h2o(base_teste[-4]))
# devemos tamb??m realizar um ajuste sobre previs??es. Se for maior do que 0.5 
# ser?? de uma classe, sen??o ser?? da outra classe
previsoes = (previsoes > 0.5)
# devemos transformas a vari??vel previs??es em um vetor, pois ele est??
# no formato environment
previsoes = as.vector(previsoes)
matriz_confusao = table(base_teste[, 4], previsoes)
library(caret)
confusionMatrix(matriz_confusao)

# ZeroR 
table(base_teste$default)
