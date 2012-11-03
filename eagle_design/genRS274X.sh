#! /bin/bash

# add 7zip and Eagle to path
PATH=/cygdrive/c/Program\ Files/7-Zip:$PATH
PATH=/cygdrive/c/Program\ Files\ \(x86\)/EAGLE-6.1.0/bin:$PATH

if [ $# -lt 1 ] ; then
    echo "usage $0 <board files...>"
    echo " e.g:  $0 boardx.brd boardy.brd"
    echo "  will create gerber files with eagle in a directory called gerbers"
    echo "  and bundle into gerbers.zip"
    exit 1
fi

for board in $@; do
    outputfile=$(basename $board .brd)
    outputdir="./tmp"

    if [ ! -d ${outputdir} ]; then
	mkdir -p ${outputdir}
    fi

    # Create the following layers:
    #  - top copper (cmp)
    #  - bottom copper (sol)
    #  - top solder mask (stc)
    #  - bottom solder mask (sts)
    #  - top silkscreen (plc)
    #  - bottom silkscreen (pls)
    #  - board outline (ger)
    eagle -X -dGERBER_RS274X -o"${outputdir}/Top Layer.cmp" ${board} Top Pads Vias
    eagle -X -dGERBER_RS274X -o"${outputdir}/Bottom Layer.sol" ${board} Bottom Pads Vias
    eagle -X -dGERBER_RS274X -o"${outputdir}/Top Solder Mask.stc" ${board} tStop
    eagle -X -dGERBER_RS274X -o"${outputdir}/Bottom Solder Mask.sts" ${board} bStop
    eagle -X -dGERBER_RS274X -o"${outputdir}/Top Silkscreen.plc" ${board} Dimension tPlace tNames
    eagle -X -dGERBER_RS274X -o"${outputdir}/Bottom Silkscreen.pls" ${board} Dimension bPlace bNames
    eagle -X -dGERBER_RS274X -o"${outputdir}/Board Outline.ger" ${board} Dimension

    # create drill files
    eagle -X -dEXCELLON -o"${outputdir}/Drills.drd" ${board} Drills Holes

    # remove unecessary files
    rm ${outputdir}/*.dri
    rm ${outputdir}/*.gpi

    # create readme
    read -d '' readme <<EOF
Layer Stack:
1 - Top silkscreen                   Top Silkscreen.plc
2 - Top solder mask                  Top Solder Mask.stc
3 - Top copper layer                 Top Layer.cmp
4 - Bottom copper layer              Bottom Layer.sol
5 - Bottom solder mask               Bottom Solder Mask.sts
6 - Bottom silkscreen                Bottom Silkscreen.pls

Board Dimensions:                    Board Outline.ger
Drill File:                          Drill.drd

Note: all layers are viewed from the TOP of the board.

Tyler Nichols
nichols.tyler@gmail.cm
801-473-1530 (cell)
EOF
    DATE=$(date --reference=${board})
    printf "Eagle .brd File: ${board}\n" > ${outputdir}/Readme.txt
    printf "Last Modified: $DATE\n\n" >> ${outputdir}/Readme.txt
    printf "$readme" >> ${outputdir}/Readme.txt

    # zip up output
    cd ${outputdir}
    7z a -tzip ${outputfile}_gerbers.zip *

    cp ${outputfile}_gerbers.zip ../
    cd ../
    rm -rf ${outputdir}
done
