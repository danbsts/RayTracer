Projeto 1 da cadeira Processamento gr�fico 2019.1
Grupo:
Rodrigo Albuquerque - rap2
Daniel Bastos - dmrb2
Ruy Melo - ropm
Christian Davi - cdbm

Objetivos atingidos:
- Parte b�sica 7pts
- Sombras 1pt
- Reflex�es Perfeitas 1pt
- Depth of Field 1pt

Instru��es de utiliza��o:

Para modificar as entradas da imagem, basta modificar o arquivo sample. Para cada modifica��o, basta seguir os passos a seguir:
Para modificar a resolu��o, basta mudar os dois valores escritos ap�s a palavra res, sendo que o primeiro valor � o valor do width e o segundo valor � o valor do length.
Para modificar as propriedades da c�mera, basta alterar os valores ap�s a palavra c�mera, estando os valores na seguinte ordem : px py pz tx ty tz ux uy uz fov f.
Para criar um novo material, basta escrever material name r g b kd ks ke alpha, substituindo name, r, g, b, kd, ks, ke, alpha por seus devidos valores.
Para criar uma nova esfera, basta escrever sphere cx cy cz r materialName, substituindo cx cy cz r materialName por seus devidos valores.
OBS: A ordem das cria��es deve ser a seguinte:
- Primeiro instanciar a resolu��o, ex: res 400 200
- Ap�s a resolu��o, instanciar a c�mera, ex: camera 10 10 10 0 0 0 0 1 0 60 0.2
- Ap�s a c�mera, instanciar os materiais, ex: material vidro 255 255 255 0.8 0.7 0.4 0.1
- Ap�s os materiais, instanciar as esferas, ex: 5 0 0 1 madeira
- Ap�s todas as cria��es, colocar a palavra fim na linha seguinte a ultima esfera

Exemplo de arquivo sample:

res 400 200
camera 10 10 10 0 0 0 0 1 0 60 0.2
material vidro 255 255 255 0.8 0.7 0.4 0.1
material madeira 200 100 23 0.8 0.7 0.4 0.1
sphere 5 0 0 1 madeira
sphere 2 0 0 1 madeira
sphere 0 0 1 1 madeira
fim

 

