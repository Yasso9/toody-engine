Toody Engine

Game engine for 2D games made with C++ and SFML

Projet d'un moteur 2D ecrit en C++ en utlisant la SFML

powershell

cd "C:\Users\Turki\GoogleDrive\Programs\Windows\Programming\C and C++\Tools\cppclean"

py cppclean --include-path=entity --include-path=graphics --include-path=input --include-path=main --include-path=output_info --include-path=ressources_handling--include-path=settings --include-path=states --include-path=tilemap --include-path=tools C:\Users\Turki\GoogleDrive\Computing\Projects\ToodyEngine\sources\project

build glad and sqlite
$(OBJECT_GLAD) :
	gcc -c $(GLAD_PATH)/glad/glad.c -o $@ -I"$(GLAD_PATH)"
$(OBJECT_SQLITE) :
	gcc -c $(SQLITE_PATH)/SQLITE/sqlite3.c -o $@ -I"$(SQLITE_PATH)"