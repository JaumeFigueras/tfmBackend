# RouterFirstVersion 

## Instalacio 

#### dependencies
les dependencies són:
Flask==1.0.2
Flask-Cors==3.0.7
Flask-RESTful==0.3.7
GDAL==2.4.0
geog==0.0.2
Jinja2==2.10
psycopg2==2.8.2
Instalar amb:
```
pip install -r requirements.txt
```
#### Posar a punt la base de dades:
Generar una bd per configurar la connexio es fa al fitxer **database.json** de **configurationFiles**

  Depositar via **osm2psql** la cartografia necesaria a la bd (amb la opcio --slim)

  Depositar la cartografia de bombers a la bd jo he empreat **Qgis**

  Extreure el osm amb els tags necesaris de la cartografia de bombers amb l'sipt **DB2OSMpaths** de **Utils/ShapeBD2OSM**

  Depositar via **osm2psql** la cartografia de bombers a la bd (amb la opcio --slim i el prefix coe)
  
  Executar el fitxer **utils/sqlFiles/inserta_taules.sql**

  Executar el script **storegraphToDB.py**

#### Compilar les llibreries de explirar en c:
Dins de router executar 
```
gcc -c -fPIC dijkstraListCFile4.cc -o dijkstraListCFile3.o
gcc -shared -W1,-soname,dijkstraListCFile3.so -o dijkstraListCFile3.so dijkstraListCFile3.o


gcc -c -fPIC dijkstraMultimodalTotal.cc -o dijkstraMultimodalTotal.o
gcc -shared -W1,-soname,dijkstraMultimodalTotal.so -o dijkstraMultimodalTotal.so dijkstraMultimodalTotal.o
```

#### Executar flask:
El port i host es poden modificar a **configurationFiles/flaskConfig.json**
executar
```
python index.py
```
