# constantes --------
GREEN='\033[0;32m'
RED='\033[0;31m'
NOCOLOR='\033[0m'
# -------------------

# Compilando o código
gcc main.c -w -o photopobre

if [ ! -d "test" ]; then
	mkdir test
	echo "Criado diretório de teste"
fi

# Testando o método inverter cores
cat lena/lena.ppm lena/inverter_cores.txt | ./photopobre > test/inverter_cores.ppm
diff lena/inverter_cores.ppm test/inverter_cores.ppm > test/inverter_cores.log
FILESIZE=$(stat -c%s test/inverter_cores.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}inverter_cores está diferente"
			echo -e "${RED}cheque test/inverter_cores.log"
		else
			echo -e "${GREEN}inverter_cores está ok"
		fi

# Testando o método blur com parâmetro 15
cat lena/lena.ppm lena/blur_15.txt | ./photopobre > test/blur_15.ppm
diff lena/blur_15.ppm test/blur_15.ppm > test/blur_15.log
FILESIZE=$(stat -c%s test/blur_15.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}blur_15 está diferente"
			echo -e "${RED}cheque test/blur_15.log"
		else
			echo -e "${GREEN}blur_15 está ok"
		fi

# Testando o método de corte
cat lena/lena.ppm lena/cortar.txt | ./photopobre > test/cortar.ppm
diff lena/cortar.ppm test/cortar.ppm > test/cortar.log
FILESIZE=$(stat -c%s test/cortar.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}cortar está diferente"
			echo -e "${RED}cheque test/cortar.log"
		else
			echo -e "${GREEN}cortar está ok"
		fi

# Testando método de escala cinza
cat lena/lena.ppm lena/escala_cinza.txt | ./photopobre > test/escala_cinza.ppm
diff lena/escala_cinza.ppm test/escala_cinza.ppm > test/escala_cinza.log
FILESIZE=$(stat -c%s test/escala_cinza.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}escala_cinza está diferente"
			echo -e "${RED}cheque test/escala_cinza.log"
		else
			echo -e "${GREEN}escala_cinza está ok"
		fi

# Testando método de espelhamento vertical
cat lena/lena.ppm lena/espelhamento_vertical.txt | ./photopobre > test/espelhamento_vertical.ppm
diff lena/espelhamento_vertical.ppm test/espelhamento_vertical.ppm > test/espelhamento_vertical.log
FILESIZE=$(stat -c%s test/espelhamento_vertical.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}espelhamento_vertical está diferente"
			echo -e "${RED}cheque test/espelhamento_vertical.log"
		else
			echo -e "${GREEN}espelhamento_vertical está ok"
		fi

# Testando método de rotação 90
cat lena/lena.ppm lena/rotacao_90.txt | ./photopobre > test/rotacao_90.ppm
diff lena/rotacao_90.ppm test/rotacao_90.ppm > test/rotacao_90.log
FILESIZE=$(stat -c%s test/rotacao_90.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}rotacao_90 está diferente"
			echo -e "${RED}cheque test/rotacao_90.log"
		else
			echo -e "${GREEN}rotacao_90 está ok"
		fi

# Testando método de sepia
cat lena/lena.ppm lena/sepia.txt | ./photopobre > test/sepia.ppm
diff lena/sepia.ppm test/sepia.ppm > test/sepia.log
FILESIZE=$(stat -c%s test/sepia.log)
if [ "$FILESIZE" -gt 0 ]; then
			echo -e "${RED}sepia está diferente"
			echo -e "${RED}cheque test/sepia.log"
		else
			echo -e "${GREEN}sepia está ok"
		fi
