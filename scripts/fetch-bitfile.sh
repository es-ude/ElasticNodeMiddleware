#!/bin/sh

if [ $1 = "4" ]; then
    src_folder=ide:git/fpgamiddlewareproject/fpgamiddlewareproject/fpgamiddlewareproject.runs/impl_2/
else
    src_folder=ide:git/fpgamiddlewareproject/
fi

echo "src_folder ${src_folder}"

#bit_file=#testProject.bit #genericProject.bit #
bit_file=$2Project.bit
# bit_file=genericProject.bit
tmp_bit_file=bit_file.bit
#program_host=arch_pi
program_port=/dev/tty.usbserial-0*

echo "fetching: $2 ${src_folder}/${bit_file}"

# fetch bit file
scp ${src_folder}/${bit_file} "$2.bit"

# upload to programming host
# scp ${tmp_bit_file} ${program_host}:${tmp_bit_file} &&
# scp $1/build/${project_name}.bit ${program_host}:bit_file.bit &&

# program fpga
# ssh ${program_host} "sudo /usr/local/bin/papilio-prog -f ${tmp_bit_file}" &&
# ssh ${program_host} "sudo /usr/local/bin/papilio-prog -f bit_file.bit"

# rm ${tmp_bit_file}
echo "Done"
