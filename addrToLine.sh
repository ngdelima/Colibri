
for call in "$@"
do
	IFS=':'
	read -a elements <<< "$call"
	xtensa-esp32-elf-addr2line -pfiaC -e build/Colibri.elf "${elements[0]}"
done