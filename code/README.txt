Projeto 1 da cadeira Processamento gráfico 2019.1
Grupo:
Rodrigo Albuquerque - rap2
Daniel Bastos - dmrb2
Ruy Melo - ropm
Christian Davi - cdbm

Objetivos atingidos:
- Parte básica 7pts
- Sombras 1pt
- Reflexões Perfeitas 1pt
- Depth of Field 1pt

Instruções de utilização:

Para modificar as entradas da imagem, basta modificar o arquivo sample. Para cada modificação, basta seguir os passos a seguir:
Para modificar a resolução, basta mudar os dois valores escritos após a palavra res, sendo que o primeiro valor é o valor do width e o segundo valor é o valor do length.
Para modificar as propriedades da câmera, basta alterar os valores após a palavra câmera, estando os valores na seguinte ordem : px py pz tx ty tz ux uy uz fov f.
Para criar um novo material, basta escrever material name r g b kd ks ke alpha, substituindo name, r, g, b, kd, ks, ke, alpha por seus devidos valores.
Para criar uma nova esfera, basta escrever sphere cx cy cz r materialName, substituindo cx cy cz r materialName por seus devidos valores.
OBS: A ordem das criações deve ser a seguinte:
- Primeiro instanciar a resolução, ex: res 400 200
- Após a resolução, instanciar a câmera, ex: camera 10 10 10 0 0 0 0 1 0 60 0.2
- Após a câmera, instanciar os materiais, ex: material vidro 255 255 255 0.8 0.7 0.4 0.1
- Após os materiais, instanciar as esferas, ex: 5 0 0 1 madeira
- Após todas as criações, colocar a palavra fim na linha seguinte a ultima esfera

Exemplo de arquivo sample:

res 400 200
camera 10 10 10 0 0 0 0 1 0 60 0.2
material vidro 255 255 255 0.8 0.7 0.4 0.1
material madeira 200 100 23 0.8 0.7 0.4 0.1
sphere 5 0 0 1 madeira
sphere 2 0 0 1 madeira
sphere 0 0 1 1 madeira
fim

 

