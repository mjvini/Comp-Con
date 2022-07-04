# Trabalho Bubble Sort Concorrente

## Descrição:
O programa tem como objetivo ordenar vetores de forma concorrente usando o método de ordenação bubble sort. O usuário digita a dimensão e o número de threads que será utilizada. O programa gera dois vetores iguais, um ele ordena de forma sequencial e o outro de forma concorrente. Ademais, ele dispõe de duas funções para verificar se os vetores foram de fato ordenados de forma crescente.

## Avaliação de desempenho:
Mostrarei aqui as medições feitas no meu computador(processador intel core i3-3240 com 2 núcleos e 4 threads; SO: Ubuntu 20.04.4 LTS). O ganho de desempenho foi calculado da mesma forma apresentada no Lab2 (Tempo sequencial / Tempo concorrente), utilizando a macro “timer.h”.

### Dimensão = 10³ Número de threads = 1:

1ª medição:
Tempo sequencial:  0.003538\
Tempo concorrente:  0.004424\
Tempo de criação das sublistas:  0.000009\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 0.799711

2ª medição:
Tempo sequencial:  0.003616\
Tempo concorrente:  0.004141\
Tempo de criação das sublistas:  0.000010\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 0.873117

3ª medição:
Tempo sequencial:  0.004729\
Tempo concorrente:  0.003940\
Tempo de criação das sublistas:  0.000014\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 1.200399

4ª medição:
Tempo sequencial:  0.002519\
Tempo concorrente:  0.005721\
Tempo de criação das sublistas:  0.000012\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 0.440277

5ª medição:
Tempo sequencial:  0.002831\
Tempo concorrente:  0.004436\
Tempo de criação das sublistas:  0.000009\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 0.638157

### Dimensão = 10³ Número de threads = 2:

1ª medição:
Tempo sequencial:  0.002923\
Tempo concorrente:  0.001629\
Tempo de criação das sublistas:  0.000034\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 1.795114

2ª medição:
Tempo sequencial:  0.003666\
Tempo concorrente:  0.001360\
Tempo de criação das sublistas:  0.000034\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 2.695362

3ª medição:
Tempo sequencial:  0.003725\
Tempo concorrente:  0.002170\
Tempo de criação das sublistas:  0.000035\
Tempo de achar o maior elemento:  0.000012\
Ganho de desempenho: 1.717071

4ª medição:
Tempo sequencial:  0.003164\
Tempo concorrente:  0.001586\
Tempo de criação das sublistas:  0.000030\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 1.995349

5ª medição:
Tempo sequencial:  0.003555\
Tempo concorrente:  0.001479\
Tempo de criação das sublistas:  0.000026\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 2.402754

### Dimensão = 10³ Número de threads = 4:

1ª medição:
Tempo sequencial:  0.003670\
Tempo concorrente:  0.001176\
Tempo de criação das sublistas:  0.000045\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 3.120193

2ª medição:
Tempo sequencial:  0.003208\
Tempo concorrente:  0.001752\
Tempo de criação das sublistas:  0.000059\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 1.830614

3ª medição:
Tempo sequencial:  0.005235\
Tempo concorrente:  0.001662\
Tempo de criação das sublistas:  0.000057\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 3.149756

4ª medição:
Tempo sequencial:  0.003640\
Tempo concorrente:  0.001268\
Tempo de criação das sublistas:  0.000049\
Tempo de achar o maior elemento:  0.000006\
Ganho de desempenho: 2.869582

5ª medição:
Tempo sequencial:  0.003177\
Tempo concorrente:  0.001489\
Tempo de criação das sublistas:  0.000058\
Tempo de achar o maior elemento:  0.000007\
Ganho de desempenho: 2.133487



### Dimensão = 10⁴ Número de threads = 1:

1ª medição:
Tempo sequencial:  0.277175\
Tempo concorrente:  0.271425\
Tempo de criação das sublistas:  0.000132\
Tempo de achar o maior elemento:  0.000050\
Ganho de desempenho: 1.021182

2ª medição:
Tempo sequencial:  0.274822\
Tempo concorrente:  0.276102\
Tempo de criação das sublistas:  0.000165\
Tempo de achar o maior elemento:  0.000047\
Ganho de desempenho: 0.995365

3ª medição:
Tempo sequencial:  0.278705\
Tempo concorrente:  0.269297\
Tempo de criação das sublistas:  0.000106\
Tempo de achar o maior elemento:  0.000040\
Ganho de desempenho: 1.034937

4ª medição:
Tempo sequencial:  0.277225\
Tempo concorrente:  0.281212\
Tempo de criação das sublistas:  0.000119\
Tempo de achar o maior elemento:  0.000045\
Ganho de desempenho: 0.985821

5ª medição:
Tempo sequencial:  0.289748\
Tempo concorrente:  0.274201\
Tempo de criação das sublistas:  0.000088\
Tempo de achar o maior elemento:  0.000029\
Ganho de desempenho: 1.056701

### Dimensão = 10⁴ Número de threads = 2:

1ª medição:
Tempo sequencial:  0.273850\
Tempo concorrente:  0.067970\
Tempo de criação das sublistas:  0.000333\
Tempo de achar o maior elemento:  0.000048\
Ganho de desempenho: 4.028975

2ª medição:
Tempo sequencial:  0.280601\
Tempo concorrente:  0.070388\
Tempo de criação das sublistas:  0.000395\
Tempo de achar o maior elemento:  0.000045\
Ganho de desempenho: 3.986479

3ª medição:
Tempo sequencial:  0.290823\
Tempo concorrente:  0.064017\
Tempo de criação das sublistas:  0.000220\
Tempo de achar o maior elemento:  0.000031\
Ganho de desempenho: 4.542904

4ª medição:
Tempo sequencial:  0.275771\
Tempo concorrente:  0.078312\
Tempo de criação das sublistas:  0.000403\
Tempo de achar o maior elemento:  0.000054\
Ganho de desempenho: 3.521464

5ª medição:
Tempo sequencial:  0.287958\
Tempo concorrente:  0.070101\
Tempo de criação das sublistas:  0.000334\
Tempo de achar o maior elemento:  0.000037\
Ganho de desempenho: 4.107737

### Dimensão = 10⁴ Número de threads = 4:

1ª medição:
Tempo sequencial:  0.277298\
Tempo concorrente:  0.076026\
Tempo de criação das sublistas:  0.000602\
Tempo de achar o maior elemento:  0.000079\
Ganho de desempenho: 3.647431

2ª medição:
Tempo sequencial:  0.288865\
Tempo concorrente:  0.064590\
Tempo de criação das sublistas:  0.000584\
Tempo de achar o maior elemento:  0.000040\
Ganho de desempenho: 4.472316

3ª medição:
Tempo sequencial:  0.279828\
Tempo concorrente:  0.069705\
Tempo de criação das sublistas:  0.000660\
Tempo de achar o maior elemento:  0.000044\
Ganho de desempenho: 4.014444

4ª medição:
Tempo sequencial:  0.279840\
Tempo concorrente:  0.073605\
Tempo de criação das sublistas:  0.000664\
Tempo de achar o maior elemento:  0.000045\
Ganho de desempenho: 3.801945

5ª medição:
Tempo sequencial:  0.284330\
Tempo concorrente:  0.069589\
Tempo de criação das sublistas:  0.000489\
Tempo de achar o maior elemento:  0.000040\
Ganho de desempenho: 4.085818

### Dimensão = 10⁵ Número de threads = 1:

1ª medição:
Tempo sequencial:  31.697388\
Tempo concorrente:  30.153166\
Tempo de criação das sublistas:  0.001243\
Tempo de achar o maior elemento:  0.000486\
Ganho de desempenho: 1.051213

2ª medição:
Tempo sequencial:  32.111489\
Tempo concorrente:  31.172223\
Tempo de criação das sublistas:  0.001486\
Tempo de achar o maior elemento:  0.000365\
Ganho de desempenho: 1.030131

3ª medição:
Tempo sequencial:  32.515883\
Tempo concorrente:  31.623134\
Tempo de criação das sublistas:  0.001042\
Tempo de achar o maior elemento:  0.000391\
Ganho de desempenho: 1.028231

4ª medição:
Tempo sequencial:  32.267610\
Tempo concorrente:  31.154974\
Tempo de criação das sublistas:  0.000921\
Tempo de achar o maior elemento:  0.000381\
Ganho de desempenho: 1.035713

5ª medição:
Tempo sequencial:  32.230681\
Tempo concorrente:  31.406329\
Tempo de criação das sublistas:  0.001288\
Tempo de achar o maior elemento:  0.000418\
Ganho de desempenho: 1.026248

### Dimensão = 10⁵ Número de threads = 2:

1ª medição:
Tempo sequencial:  31.079208\
Tempo concorrente:  8.620535\
Tempo de criação das sublistas:  0.002715\
Tempo de achar o maior elemento:  0.000407\
Ganho de desempenho: 3.605253

2ª medição:
Tempo sequencial:  31.645968\
Tempo concorrente:  7.746057\
Tempo de criação das sublistas:  0.003073\
Tempo de achar o maior elemento:  0.000382\
Ganho de desempenho: 4.085429

3ª medição:
Tempo sequencial:  31.693490\
Tempo concorrente:  8.901567\
Tempo de criação das sublistas:  0.002499\
Tempo de achar o maior elemento:  0.000346\
Ganho de desempenho: 3.560439

4ª medição:
Tempo sequencial:  32.482831\
Tempo concorrente:  7.957529\
Tempo de criação das sublistas:  0.003487\
Tempo de achar o maior elemento:  0.000362\
Ganho de desempenho: 4.082025

5ª medição:
Tempo sequencial:  31.614369\
Tempo concorrente:  7.945392\
Tempo de criação das sublistas:  0.002403\
Tempo de achar o maior elemento:  0.000297\
Ganho de desempenho: 3.978957

### Dimensão = 10⁵ Número de threads = 4:

1ª medição:
Tempo sequencial:  31.323581\
Tempo concorrente:  8.097519\
Tempo de criação das sublistas:  0.004881\
Tempo de achar o maior elemento:  0.000356\
Ganho de desempenho: 3.868294

2ª medição:
Tempo sequencial:  32.268335\
Tempo concorrente:  7.870958\
Tempo de criação das sublistas:  0.004431\
Tempo de achar o maior elemento:  0.000443\
Ganho de desempenho: 4.099671

3ª medição:
Tempo sequencial:  31.688635\
Tempo concorrente:  7.916724\
Tempo de criação das sublistas:  0.004170\
Tempo de achar o maior elemento:  0.000414\
Ganho de desempenho: 4.002746

4ª medição:
Tempo sequencial:  31.471950\
Tempo concorrente:  8.275303\
Tempo de criação das sublistas:  0.004661\
Tempo de achar o maior elemento:  0.000449\
Ganho de desempenho: 3.803117

5ª medição:
Tempo sequencial:  31.370723\
Tempo concorrente:  7.968614\
Tempo de criação das sublistas:  0.004735\
Tempo de achar o maior elemento:  0.000442\
Ganho de desempenho: 3.936786

## Discussão:

Os resultados obtidos estão de acordo com o esperado, já que a implementação concorrente está mais rápida do que a sequencial conforme a dimensão do vetor e o número de threads aumentam.
