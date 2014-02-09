/*******************************************************************************
 * XEOS - X86 Experimental Operating System
 * 
 * Copyright (c) 2010-2013, Jean-David Gadina - www.xs-labs.com
 * All rights reserved.
 * 
 * XEOS Software License - Version 1.0 - December 21, 2012
 * 
 * Permission is hereby granted, free of charge, to any person or organisation
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to deal in the Software, with or without
 * modification, without restriction, including without limitation the rights
 * to use, execute, display, copy, reproduce, transmit, publish, distribute,
 * modify, merge, prepare derivative works of the Software, and to permit
 * third-parties to whom the Software is furnished to do so, all subject to the
 * following conditions:
 * 
 *      1.  Redistributions of source code, in whole or in part, must retain the
 *          above copyright notice and this entire statement, including the
 *          above license grant, this restriction and the following disclaimer.
 * 
 *      2.  Redistributions in binary form must reproduce the above copyright
 *          notice and this entire statement, including the above license grant,
 *          this restriction and the following disclaimer in the documentation
 *          and/or other materials provided with the distribution, unless the
 *          Software is distributed by the copyright owner as a library.
 *          A "library" means a collection of software functions and/or data
 *          prepared so as to be conveniently linked with application programs
 *          (which use some of those functions and data) to form executables.
 * 
 *      3.  The Software, or any substancial portion of the Software shall not
 *          be combined, included, derived, or linked (statically or
 *          dynamically) with software or libraries licensed under the terms
 *          of any GNU software license, including, but not limited to, the GNU
 *          General Public License (GNU/GPL) or the GNU Lesser General Public
 *          License (GNU/LGPL).
 * 
 *      4.  All advertising materials mentioning features or use of this
 *          software must display an acknowledgement stating that the product
 *          includes software developed by the copyright owner.
 * 
 *      5.  Neither the name of the copyright owner nor the names of its
 *          contributors may be used to endorse or promote products derived from
 *          this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT OWNER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE AND NON-INFRINGEMENT ARE DISCLAIMED.
 * 
 * IN NO EVENT SHALL THE COPYRIGHT OWNER, CONTRIBUTORS OR ANYONE DISTRIBUTING
 * THE SOFTWARE BE LIABLE FOR ANY CLAIM, DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN ACTION OF CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

/* $Id$ */

#include "elf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char * argv[] )
{
    FILE * fh;
    char * data;
    char * buf;
    size_t size;
    size_t alloc;
    bool   showStrings;
    bool   showSymbols;
    
    fh   = NULL;
    data = NULL;
    
    if( argc < 2 )
    {
        {
            char * exec;
            
            exec = strrchr( argv[ 0 ], '/' );
            
            if( exec != argv[ 0 ] )
            {
                exec++;
            }
            
            printf
            (
                "Usage: %s [OPTIONS] <elf-file>\n"
                "\n"
                "Options:\n"
                "\n"
                "    --all          Displays everything\n"
                "    --strings      Displays the content of string tables\n"
                "    --symbols      Displays the content of symbol tables\n",
                exec
            );
        }
        
        return EXIT_SUCCESS;
    }
    
    showStrings = false;
    showSymbols = false;
    
    {
        int arg;
        
        for( arg = 1; arg < argc; arg++ )
        {
            if( strcmp( argv[ arg ], "--strings" ) == 0 )
            {
                showStrings = true;
            }
            
            if( strcmp( argv[ arg ], "--symbols" ) == 0 )
            {
                showSymbols = true;
            }
            
            if( strcmp( argv[ arg ], "--all" ) == 0 )
            {
                showStrings = true;
                showSymbols = true;
            }
        }
    }
    
    if( ( fh = fopen( argv[ argc - 1 ], "r" ) ) == NULL )
    {
        goto error;
    }
    
    if( ( data = malloc( 4096 ) ) == NULL )
    {
        goto error;
    }
    
    buf   = data;
    alloc = 1;
    size  = alloc;
    
    while( fread( buf, 1, alloc, fh ) )
    {
        size += alloc;
        
        if( ( data = realloc( data, size ) ) == NULL )
        {
            goto error;
        }
        
        buf = data + ( size - alloc );
    }
    
    {
        ELF64_FileRef               file;
        ELF64_HeaderRef             header;
        ELF64_ProgramHeaderEntryRef pHeader;
        ELF64_SectionHeaderEntryRef sHeader;
        ELF64_Half                  i;
        
        file    = ELF64_ReadFromData( data );
        header  = ELF64_FileGetHeader( file );
        
        if( ELF64_FileIsValid( file ) == false )
        {
            printf( "Invalid ELF64 file...\n" );
            
            goto end;
        }
        
        printf
        (
            "ELF64 File:\n"
            "===========\n"
            "\n"
            "Header infos:\n"
            "-------------\n"
            "\n"
            "    - Object file class:                  %s\n"
            "    - Data encoding:                      %s\n"
            "    - File version:                       %u\n"
            "    - OS ABI:                             %s\n"
            "    - Object file type:                   %s\n"
            "    - Version:                            %u\n"
            "    - Machine:                            %s\n"
            "    - Entry point address:                0x%016llX\n"
            "    - Program header offset:              %lu\n"
            "    - Section header offset:              %lu\n"
            "    - Processor-specific flags:           %lu\n"
            "    - ELF header size:                    %lu\n"
            "    - Size of program header entry:       %lu\n"
            "    - Number of program header entries:   %lu\n"
            "    - Size of section header entry:       %lu\n"
            "    - Number of section header entries:   %lu\n"
            "    - Section name string table index:    %lu\n",
            
            ELF64_HeaderGetObjectFileClassString( header ),
            ELF64_HeaderGetDataEncodingString( header ),
            ELF64_HeaderGetFileVersion( header ),
            ELF64_HeaderGetOSABIString( header ),
            ELF64_HeaderGetObjectFileTypeString( header ),
            ELF64_HeaderGetVersion( header ),
            ELF64_HeaderGetMachineTypeString( header ),
            ELF64_HeaderGetEntryPointAddress( header ),
            ( unsigned long )ELF64_HeaderGetProgramHeaderOffset( header ),
            ( unsigned long )ELF64_HeaderGetSectionHeaderOffset( header ),
            ( unsigned long )ELF64_HeaderGetProcessorSpecificFlags( header ),
            ( unsigned long )ELF64_HeaderGetHeaderSize( header ),
            ( unsigned long )ELF64_HeaderGetProgramHeaderEntrySize( header ),
            ( unsigned long )ELF64_HeaderGetProgramHeaderEntryCount( header ),
            ( unsigned long )ELF64_HeaderGetSectionHeaderEntrySize( header ),
            ( unsigned long )ELF64_HeaderGetSectionHeaderEntryCount( header ),
            ( unsigned long )ELF64_HeaderGetSectionNameStringTableIndex( header )
        );
        
        printf
        (
            "\n"
            "Program header entries:\n"
            "-----------------------\n"
            "\n"
        );
        
        for( i = 0; i < ELF64_HeaderGetProgramHeaderEntryCount( header ); i++ )
        {
            pHeader = ELF64_FileGetProgramHeaderEntry( file, i );
            
            printf
            (
                "    ### Program header entry (%lu):\n"
                "    \n"
                "        - Segment type:                %s\n"
                "        - Segment attributes:          %lu (%s)\n"
                "        - Offset in file:              0x%016llX\n"
                "        - Virtual address in memory:   0x%016llX\n"
                "        - Size of segment in file:     %lu\n"
                "        - Size of segment in memory:   %lu\n"
                "        - Alignment of segment:        %lu\n"
                "    \n",
                ( unsigned long )i,
                ELF64_ProgramHeaderEntryGetTypeString( pHeader ),
                ( unsigned long )ELF64_ProgramHeaderEntryGetAttributes( pHeader ),
                ELF64_ProgramHeaderEntryGetAttributesString( pHeader ),
                ELF64_ProgramHeaderEntryGetFileOffset( pHeader ),
                ELF64_ProgramHeaderEntryGetMemoryVirtualAddress( pHeader ),
                ( unsigned long )ELF64_ProgramHeaderEntryGetFileSize( pHeader ),
                ( unsigned long )ELF64_ProgramHeaderEntryGetMemorySize( pHeader ),
                ( unsigned long )ELF64_ProgramHeaderEntryGetAlignment( pHeader )
            );
        }
        
        printf
        (
            "Section header entries:\n"
            "-----------------------\n"
            "\n"
        );
        
        for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ); i++ )
        {
            sHeader = ELF64_FileGetSectionHeaderEntry( file, i );
            
            printf
            (
                "    ### Section header entry (%lu):\n"
                "    \n"
                "        - Section name:                            %s\n"
                "        - Section type:                            %s\n"
                "        - Section attributes:                      %lu (%s)\n"
                "        - Virtual address in memory:               0x%016llX\n"
                "        - Offset in file:                          %lu\n"
                "        - Size of section:                         %lu\n"
                "        - Link to other section:                   %lu\n"
                "        - Miscellaneous information:               %lu\n"
                "        - Address alignment boundary:              %lu\n"
                "        - Size of entries, if section has table:   %lu\n"
                "    \n",
                ( unsigned long )i,
                ELF64_FileGetNameOfSection( file, sHeader ),
                ELF64_SectionHeaderEntryGetTypeString( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetAttributes( sHeader ),
                ELF64_SectionHeaderEntryGetAttributesString( sHeader ),
                ELF64_SectionHeaderEntryGetMemoryVirtualAddress( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetFileOffset( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetSectionSize( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetLinkedSectionIndex( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetMiscInfo( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetAddressAlignmentBoundary( sHeader ),
                ( unsigned long )ELF64_SectionHeaderEntryGetEntrySize( sHeader )
            );
        }
        
        for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ) && showStrings == true; i++ )
        {
            sHeader = ELF64_FileGetSectionHeaderEntry( file, i );
            
            if( ELF64_SectionHeaderEntryGetType( sHeader ) != ELF64_SectionTypeStringTable )
            {
                continue;
            }
            
            printf
            (
                "String table:\n"
                "-------------\n"
                "\n"
                "    - Section number: %lu\n"
                "    - Section name:   %s\n"
                "\n",
                ( unsigned long )i,
                ELF64_FileGetNameOfSection( file, sHeader )
            );
            
            {
                ELF64_XWord   j;
                ELF64_XWord   totalLength;
                ELF64_XWord   stringLength;
                ELF64_XWord   sectionSize;
                ELF64_UChar * sectionData;
                
                sectionSize = ELF64_SectionHeaderEntryGetSectionSize( sHeader );
                sectionData = ELF64_FileGetDataForSection( file, sHeader );
                totalLength = 0;
                j           = 0;
                
                printf
                (
                    "    ### Strings:\n"
                    "    \n"
                );
                
                while( totalLength < sectionSize )
                {
                    stringLength = strlen( ( const char * )sectionData );
                    
                    if( stringLength > 0 )
                    {
                        printf
                        (
                            "        - %010lu (%010lu): %s\n",
                            ( unsigned long )j,
                            ( unsigned long )totalLength,
                            sectionData
                        );
                    }
                    
                    sectionData += stringLength + 1;
                    totalLength += stringLength + 1;
                    
                    j++;
                }
                
                printf( "\n" );
            }
        }
            
        for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ) && showSymbols == true; i++ )
        {
            sHeader = ELF64_FileGetSectionHeaderEntry( file, i );
            
            if( ELF64_SectionHeaderEntryGetType( sHeader ) != ELF64_SectionTypeLinkerSymbolTable )
            {
                continue;
            }
            
            printf
            (
                "Linker symbol table:\n"
                "--------------------\n"
                "\n"
                "    - Section number: %lu\n"
                "    - Section name:   %s\n"
                "\n",
                ( unsigned long )i,
                ELF64_FileGetNameOfSection( file, sHeader )
            );
            
            {
                unsigned int                symCount;
                unsigned int                j;
                ELF64_SymbolTableEntryRef   sym;
                
                symCount = ELF64_SectionHeaderEntryGetSymbolTableEntryCount( sHeader );
                
                for( j = 0; j < symCount; j++ )
                {
                    sym = ELF64_FileGetSymbolTableEntryForSection( file, sHeader, j );
                    
                    printf
                    (
                        "    ### Symbol (%lu):\n"
                        "    \n"
                        "        - Symbol name:                 %s\n"
                        "        - Symbol type:                 %s\n"
                        "        - Symbol bindings:             %s\n"
                        "        - Section table index:         %lu\n"
                        "        - Symbol value:                0x%016llX\n"
                        "        - Size of object:              %lu\n"
                        "    \n",
                        ( unsigned long )j,
                        ELF64_FileGetSymbolTableEntryNameForSection( file, sym, sHeader ),
                        ELF64_SymbolTableEntryGetTypeString( sym ),
                        ELF64_SymbolTableEntryGetBindingString( sym ),
                        ( unsigned long )ELF64_SymbolTableEntryGetSectionTableIndex( sym ),
                        ELF64_SymbolTableEntryGetSymbolValue( sym ),
                        ( unsigned long )ELF64_SymbolTableEntryGetObjectSize( sym )
                    );
                }
            }
        }
    }
    
    goto end;
    
    error:
        
        printf( "Error reading ELF file: %s\n", argv[ argc - 1 ] );
        
    end:
        
        free( data );
        fclose( fh );
    
    return EXIT_SUCCESS;
}

