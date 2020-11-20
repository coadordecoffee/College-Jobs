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

# Encode para a classe
base$default = factor(base$default, levels = c(0,1))

# Divis??o entre treinamento e teste
library(caTools)
set.seed(1)
divisao = sample.split(base$income, SplitRatio = 0.75)
base_treinamento = subset(base, divisao == TRUE)
base_teste = subset(base, divisao == FALSE)

#install.packages('randomForest')
library(randomForest)
set.seed(1)

# os par??metros da fun????o randon s??o: x ( a matriz dos atributos previsores), y (classe alvo) e
# ntree (n??mero de ??rvores)
classificador = randomForest(x = base_treinamento[-4], y = base_treinamento$default, ntree = 16)
# N??mero de vari??veis amostradas aleatoriamente como candidatos em cada divis??o. 
# Note que os valores padr??o s??o diferentes para classifica????o (sqrt (p) 
# onde p ?? o n??mero de vari??veis em x) e regress??o (p / 3)

# temos aqui um novo argumento na fun????o predict, o type que vai receber 'class'
# deve ser feito isso para que ele retorne somente o valor da classe e n??o da probabilidade.
previsoes = predict(classificador, newdata = base_teste[-4], type = 'class')
matriz_confusao = table(base_teste[, 4], previsoes)
print(matriz_confusao)
#install.packages('data.table')
#install.packages('robustbase')
#install.packages('caret')
library(caret)
confusionMatrix(matriz_confusao)
