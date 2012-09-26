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
    eagle -X -dGERBER_RS274X -o"${outputdir}/Top Silkscreen.plc" ${board} Dimension tPlace tNames tValues
    eagle -X -dGERBER_RS274X -o"${outputdir}/Bottom Silkscreen.pls" ${board} Dimension bPlace bNames bValues
    eagle -X -dGERBER_RS274X -o"${outputdir}/Board Outline.ger" ${board} Dimension
    
    # create drill files
    eagle -X -dEXCELLON -o${outputdir}/Drills.drd ${board} Drills Holes
    rm ${outputdir}/*.drd
    # get drills used from dri file and create drl rack file
    # (like running drillcfg.ulp: ${EAGLE} -N- -C'RUN drillcfg.ulp; QUIT;' -o${outputfile} ${board}
    cat ${outputdir}/Drills.dri | \
	sed -e 's/ *\(T[0-9][0-9]\) *\([0-9.]*..\).*/\1 \2/' | \
	grep -e "^T[0-9][0-9]" > ${outputdir}/Drills.drd
    
    # remove unecessary files
    rm ${outputdir}/*.dri
    rm ${outputdir}/*.gpi
    
    # zip up output
    cd ${outputdir}
    7z a -tzip ${outputfile}_gerbers.zip *
    
    cp ${outputfile}_gerbers.zip ../
    cd ../
    rm -rf ${outputdir}
done