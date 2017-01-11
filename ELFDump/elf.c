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
#include <string.h>

struct __ELF64_SymbolTableEntry
{
    ELF64_Word  st_name;    /* Symbol name */
    ELF64_UChar st_info;    /* Type and binding attributes */
    ELF64_UChar st_other;   /* Reserved */
    ELF64_Half  st_shndx;   /* Section table index */
    ELF64_Addr  st_value;   /* Symbol value */
    ELF64_XWord st_size;    /* Size of object */
};

struct __ELF64_ProgramHeaderEntry
{
    ELF64_Word  p_type;     /* Type of segment */
    ELF64_Word  p_flags;    /* Segment attributes */
    ELF64_Off   p_offset;   /* Offset in file */
    ELF64_Addr  p_vaddr;    /* Virtual address in memory */
    ELF64_Addr  p_paddr;    /* Reserved */
    ELF64_XWord p_filesz;   /* Size of segment in file */
    ELF64_XWord p_memsz;    /* Size of segment in memory */
    ELF64_XWord p_align;    /* Alignment of segment */
};

struct __ELF64_SectionHeaderEntry
{
    ELF64_Word  sh_name;        /* Section name */
    ELF64_Word  sh_type;        /* Section type */
    ELF64_XWord sh_flags;       /* Section attributes */
    ELF64_Addr  sh_addr;        /* Virtual address in memory */
    ELF64_Off   sh_offset;      /* Offset in file */
    ELF64_XWord sh_size;        /* Size of section */
    ELF64_Word  sh_link;        /* Link to other section */
    ELF64_Word  sh_info;        /* Miscellaneous information */
    ELF64_XWord sh_addralign;   /* Address alignment boundary */
    ELF64_XWord sh_entsize;     /* Size of entries, if section has table */
};

struct __ELF64_Header
{
    ELF64_UChar   e_ident[ 16 ];  /* ELF identification */
    ELF64_Half    e_type;         /* Object file type */
    ELF64_Half    e_machine;      /* Machine type */
    ELF64_Word    e_version;      /* Object file version */
    ELF64_Addr    e_entry;        /* Entry point address */
    ELF64_Off     e_phoff;        /* Program header offset */
    ELF64_Off     e_shoff;        /* Section header offset */
    ELF64_Word    e_flags;        /* Processor-specific flags */
    ELF64_Half    e_ehsize;       /* ELF header size */
    ELF64_Half    e_phentsize;    /* Size of program header entry */
    ELF64_Half    e_phnum;        /* Number of program header entries */
    ELF64_Half    e_shentsize;    /* Size of section header entry */
    ELF64_Half    e_shnum;        /* Number of section header entries */
    ELF64_Half    e_shstrndx;     /* Section name string table index */
};

struct __ELF64_File
{
    struct __ELF64_Header header;
};

ELF64_FileRef ELF64_ReadFromData( char * data )
{
    return ( ELF64_FileRef )( ( void * )data );
}

bool ELF64_FileIsValid( ELF64_FileRef file )
{
    if( file == NULL )
    {
        return false;
    }
    
    if
    (
           file->header.e_ident[ 0 ] == 0x7F
        && file->header.e_ident[ 1 ] == 0x45    /* E */
        && file->header.e_ident[ 2 ] == 0x4C    /* L */
        && file->header.e_ident[ 3 ] == 0x46    /* F */
    )
    {
        return true;
    }
    
    return false;
}

ELF64_HeaderRef ELF64_FileGetHeader( ELF64_FileRef file )
{
    if( file == NULL )
    {
        return NULL;
    }
    
    return &( file->header );
}

ELF64_ProgramHeaderEntryRef ELF64_FileGetProgramHeaderEntry( ELF64_FileRef file, unsigned int index )
{
    ELF64_HeaderRef header;
    ELF64_Off       offset;
    ELF64_Half      entrySize;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL || index >= ELF64_HeaderGetProgramHeaderEntryCount( header ) )
    {
        return NULL;
    }
    
    offset      = ELF64_HeaderGetProgramHeaderOffset( header );
    entrySize   = ELF64_HeaderGetProgramHeaderEntrySize( header );
    
    return ( ELF64_ProgramHeaderEntryRef )( ( void * )( ( char * )file + offset + ( entrySize * index ) ) );
}

ELF64_SectionHeaderEntryRef ELF64_FileGetSectionHeaderEntry( ELF64_FileRef file, unsigned int index )
{
    ELF64_HeaderRef header;
    ELF64_Off       offset;
    ELF64_Half      entrySize;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL || index >= ELF64_HeaderGetSectionHeaderEntryCount( header ) )
    {
        return NULL;
    }
    
    offset      = ELF64_HeaderGetSectionHeaderOffset( header );
    entrySize   = ELF64_HeaderGetSectionHeaderEntrySize( header );
    
    return ( ELF64_SectionHeaderEntryRef )( ( void * )( ( char * )file + offset + ( entrySize * index ) ) );
}

const char * ELF64_FileGetNameOfSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section )
{
    ELF64_HeaderRef             header;
    ELF64_SectionHeaderEntryRef stringTable;
    ELF64_Word                  nameOffset;
    
    if( file == NULL || section == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL )
    {
        return NULL;
    }
    
    stringTable = ELF64_FileGetStringTableHeaderEntry( file );
    nameOffset  = ELF64_SectionHeaderEntryGetNameOffset( section );
    
    return ( const char * )( ELF64_FileGetDataForSection( file, stringTable ) + nameOffset );
}

const char * ELF64_FileGetNameOfSectionAtIndex( ELF64_FileRef file, unsigned int index )
{
    return ELF64_FileGetNameOfSection( file, ELF64_FileGetSectionHeaderEntry( file, index ) );
}

ELF64_UChar * ELF64_FileGetDataForSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section )
{
    ELF64_Off fileOffset;
    
    if( section == NULL )
    {
        return NULL;
    }
    
    fileOffset = ELF64_SectionHeaderEntryGetFileOffset( section );
    
    return ( ELF64_UChar * )file + fileOffset;
}

ELF64_UChar * ELF64_FileGetDataForSectionAtIndex( ELF64_FileRef file, unsigned int index )
{
    return ELF64_FileGetDataForSection( file, ELF64_FileGetSectionHeaderEntry( file, index ) );
}

ELF64_SymbolTableEntryRef ELF64_FileGetSymbolTableEntryForSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section, unsigned int index )
{
    ELF64_XWord entrySize;
    
    if( ELF64_SectionHeaderEntryGetType( section ) != ELF64_SectionTypeLinkerSymbolTable )
    {
        return NULL;
    }
    
    if( index >= ELF64_SectionHeaderEntryGetSymbolTableEntryCount( section ) )
    {
        return NULL;
    }
    
    entrySize = ELF64_SectionHeaderEntryGetEntrySize( section );
    
    return ( ELF64_SymbolTableEntryRef )( ( void * )( ELF64_FileGetDataForSection( file, section ) + ( index * entrySize ) ) );
}

const char * ELF64_FileGetSymbolTableEntryNameForSection( ELF64_FileRef file, ELF64_SymbolTableEntryRef sym, ELF64_SectionHeaderEntryRef section )
{
    ELF64_Word                  stringTableIndex;
    ELF64_SectionHeaderEntryRef stringTable;
    ELF64_Word                  nameOffset;
    const char                * name;
    
    if( file == NULL || sym == NULL || section == NULL )
    {
        return NULL;
    }
    
    stringTableIndex = ELF64_SectionHeaderEntryGetLinkedSectionIndex( section );
    stringTable      = ELF64_FileGetSectionHeaderEntry( file, stringTableIndex );
    
    if( stringTable == NULL )
    {
        return NULL;
    }
    
    nameOffset = ELF64_SymbolTableEntryGetNameOffset( sym );
    name       = ( const char * )( ELF64_FileGetDataForSection( file, stringTable ) + nameOffset );
    
    if( strlen( name ) == 0 )
    {
        return "<unnamed-symbol>";
    }
    
    return name;
}

ELF64_SectionHeaderEntryRef ELF64_FileGetStringTableHeaderEntry( ELF64_FileRef file )
{
    ELF64_HeaderRef header;
    ELF64_Half      stringTableIndex;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL )
    {
        return NULL;
    }
    
    stringTableIndex = ELF64_HeaderGetSectionNameStringTableIndex( header );
    
    return ELF64_FileGetSectionHeaderEntry( file, stringTableIndex );
}

ELF64_SectionHeaderEntryRef ELF64_FileGetSectionHeaderEntryWithName( ELF64_FileRef file, const char * name )
{
    ELF64_Half                  i;
    ELF64_HeaderRef             header;
    ELF64_SectionHeaderEntryRef section;
    const char                * sName;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL )
    {
        return NULL;
    }
    
    for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ); i++ )
    {
        section = ELF64_FileGetSectionHeaderEntry( file, i );
        sName   = ELF64_FileGetNameOfSection( file, section );
        
        if( strcmp( name, sName ) == 0 )
        {
            return section;
        }
    }
    
    return NULL;
}

ELF64_SymbolTableEntryRef ELF64_FileGetSymbolTableEntryWithName( ELF64_FileRef file, const char * name )
{
    ELF64_HeaderRef             header;
    ELF64_SectionHeaderEntryRef sHeader;
    ELF64_Half                  i;
    unsigned int                symCount;
    unsigned int                j;
    ELF64_SymbolTableEntryRef   sym;
    const char                * symName;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL )
    {
        return NULL;
    }
    
    for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ); i++ )
    {
        sHeader = ELF64_FileGetSectionHeaderEntry( file, i );
        
        if( ELF64_SectionHeaderEntryGetType( sHeader ) != ELF64_SectionTypeLinkerSymbolTable )
        {
            continue;
        }
        
        symCount = ELF64_SectionHeaderEntryGetSymbolTableEntryCount( sHeader );
            
        for( j = 0; j < symCount; j++ )
        {
            sym     = ELF64_FileGetSymbolTableEntryForSection( file, sHeader, j );
            symName = ELF64_FileGetSymbolTableEntryNameForSection( file, sym, sHeader );
            
            if( strcmp( name, symName ) == 0 )
            {
                return sym;
            }
        }
    }
    
    return NULL;
}

ELF64_SymbolTableEntryRef ELF64_FileGetSymbolTableEntryWithAddress( ELF64_FileRef file, ELF64_Addr address )
{
    ELF64_HeaderRef             header;
    ELF64_SectionHeaderEntryRef sHeader;
    ELF64_Half                  i;
    unsigned int                symCount;
    unsigned int                j;
    ELF64_SymbolTableEntryRef   sym;
    ELF64_Addr                  symAddress;
    
    if( file == NULL )
    {
        return NULL;
    }
    
    header = ELF64_FileGetHeader( file );
    
    if( header == NULL )
    {
        return NULL;
    }
    
    for( i = 0; i < ELF64_HeaderGetSectionHeaderEntryCount( header ); i++ )
    {
        sHeader = ELF64_FileGetSectionHeaderEntry( file, i );
        
        if( ELF64_SectionHeaderEntryGetType( sHeader ) != ELF64_SectionTypeLinkerSymbolTable )
        {
            continue;
        }
        
        symCount = ELF64_SectionHeaderEntryGetSymbolTableEntryCount( sHeader );
            
        for( j = 0; j < symCount; j++ )
        {
            sym         = ELF64_FileGetSymbolTableEntryForSection( file, sHeader, j );
            symAddress  = ELF64_SymbolTableEntryGetSymbolValue( sym );
            
            if( address == symAddress )
            {
                return sym;
            }
        }
    }
    
    return NULL;
}

ELF64_ObjectFileClass ELF64_HeaderGetObjectFileClass( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return ELF64_ObjectFileClassUnknown;
    }
    
    switch( header->e_ident[ 4 ] )
    {
        case 1:     return ELF64_ObjectFileClass32;
        case 2:     return ELF64_ObjectFileClass64;
        default:    break;
    }
    
    return ELF64_ObjectFileClassUnknown;
}

const char * ELF64_HeaderGetObjectFileClassString( ELF64_HeaderRef header )
{
    switch( ELF64_HeaderGetObjectFileClass( header ) )
    {
        case ELF64_ObjectFileClassUnknown:  return "Unknown";
        case ELF64_ObjectFileClass32:       return "32 bits";
        case ELF64_ObjectFileClass64:       return "64 bits";
    }
}

ELF64_DataEncoding ELF64_HeaderGetDataEncoding( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return ELF64_DataEncodingUnknown;
    }
    
    switch( header->e_ident[ 5 ] )
    {
        case 1:     return ELF64_DataEncodingLSB;
        case 2:     return ELF64_DataEncodingMSB;
        default:    break;
    }
    
    return ELF64_DataEncodingUnknown;
}

const char * ELF64_HeaderGetDataEncodingString( ELF64_HeaderRef header )
{
    switch( ELF64_HeaderGetDataEncoding( header ) )
    {
        case ELF64_DataEncodingUnknown: return "LSB";
        case ELF64_DataEncodingLSB:     return "LSB";
        case ELF64_DataEncodingMSB:     return "MSB";
    }
}

ELF64_UChar ELF64_HeaderGetFileVersion( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_ident[ 6 ];
}

ELF64_OSABI ELF64_HeaderGetOSABI( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return ELF64_OSABIUnknown;
    }
    
    switch( header->e_ident[ 8 ] )
    {
        case 0:     return ELF64_OSABISysV;
        case 1:     return ELF64_OSABIHPUX;
        case 255:   return ELF64_OSABIStandalone;
        default:    break;
    }
    
    return ELF64_OSABIUnknown;
}

const char * ELF64_HeaderGetOSABIString( ELF64_HeaderRef header )
{
    switch( ELF64_HeaderGetOSABI( header ) )
    {
        case ELF64_OSABIUnknown:    return "Unknown";
        case ELF64_OSABISysV:       return "System V";
        case ELF64_OSABIHPUX:       return "HP/UX";
        case ELF64_OSABIStandalone: return "Standalone";
    }
}

ELF64_ObjectFileType ELF64_HeaderGetObjectFileType( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return ELF64_ObjectFileTypeNone;
    }
    
    switch( header->e_type )
    {
        case 0:     return ELF64_ObjectFileTypeNone;
        case 1:     return ELF64_ObjectFileTypeRelocatableObjectFile;
        case 2:     return ELF64_ObjectFileTypeExecutableFile;
        case 3:     return ELF64_ObjectFileTypeSharedObjectFile;
        case 4:     return ELF64_ObjectFileTypeCoreFile;
        default:    break;
    }
    
    return ELF64_ObjectFileTypeNone;
}

const char * ELF64_HeaderGetObjectFileTypeString( ELF64_HeaderRef header )
{
    switch( ELF64_HeaderGetObjectFileType( header ) )
    {
        case ELF64_ObjectFileTypeNone:                  return "None";
        case ELF64_ObjectFileTypeRelocatableObjectFile: return "Relocatable object file";
        case ELF64_ObjectFileTypeExecutableFile:        return "Executable file";
        case ELF64_ObjectFileTypeSharedObjectFile:      return "Shared object file";
        case ELF64_ObjectFileTypeCoreFile:              return "Core file";
    }
}

ELF64_Half ELF64_HeaderGetMachine( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_machine;
}

ELF64_MachineType ELF64_HeaderGetMachineType( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return ELF64_MachineTypeUnknown;
    }
    
    switch( ELF64_HeaderGetMachine( header ) )
    {
        case 0:     return ELF64_MachineTypeUnknown;
        case 1:     return ELF64_MachineTypeM32;
        case 2:     return ELF64_MachineTypeSPARC;
        case 3:     return ELF64_MachineType386;
        case 4:     return ELF64_MachineType68K;
        case 5:     return ELF64_MachineType88K;
        case 7:     return ELF64_MachineType860;
        case 8:     return ELF64_MachineTypeMIPS;
        case 9:     return ELF64_MachineTypeS370;
        case 10:    return ELF64_MachineTypeMIPSRS3LE;
        case 15:    return ELF64_MachineTypePARISC;
        case 17:    return ELF64_MachineTypeVPP500;
        case 18:    return ELF64_MachineTypeSPARC32Plus;
        case 19:    return ELF64_MachineType960;
        case 20:    return ELF64_MachineTypePPC;
        case 21:    return ELF64_MachineTypePPC64;
        case 36:    return ELF64_MachineTypeV800;
        case 37:    return ELF64_MachineTypeFR20;
        case 38:    return ELF64_MachineTypeRH32;
        case 39:    return ELF64_MachineTypeRCE;
        case 40:    return ELF64_MachineTypeARM;
        case 41:    return ELF64_MachineTypeAlpha;
        case 42:    return ELF64_MachineTypeSH;
        case 43:    return ELF64_MachineTypeSPARCV9;
        case 44:    return ELF64_MachineTypeTricore;
        case 45:    return ELF64_MachineTypeARC;
        case 46:    return ELF64_MachineTypeH8300;
        case 47:    return ELF64_MachineTypeH8300H;
        case 48:    return ELF64_MachineTypeH8S;
        case 49:    return ELF64_MachineTypeH8500;
        case 50:    return ELF64_MachineTypeIA64;
        case 51:    return ELF64_MachineTypeMIPSX;
        case 52:    return ELF64_MachineTypeColdFire;
        case 53:    return ELF64_MachineType68HC12;
        case 54:    return ELF64_MachineTypeMMA;
        case 55:    return ELF64_MachineTypePCP;
        case 56:    return ELF64_MachineTypeNCPU;
        case 57:    return ELF64_MachineTypeNDR1;
        case 58:    return ELF64_MachineTypeStarCore;
        case 59:    return ELF64_MachineTypeME16;
        case 60:    return ELF64_MachineTypeST100;
        case 61:    return ELF64_MachineTypeTinyJ;
        case 66:    return ELF64_MachineTypeFX66;
        case 67:    return ELF64_MachineTypeST9Plus;
        case 68:    return ELF64_MachineTypeST7;
        case 69:    return ELF64_MachineType68HC16;
        case 70:    return ELF64_MachineType68HC11;
        case 71:    return ELF64_MachineType68HC08;
        case 72:    return ELF64_MachineType68HC05;
        case 73:    return ELF64_MachineTypeSVX;
        case 74:    return ELF64_MachineTypeST19;
        case 75:    return ELF64_MachineTypeVAX;
        case 76:    return ELF64_MachineTypeCRIS;
        case 77:    return ELF64_MachineTypeJavelin;
        case 78:    return ELF64_MachineTypeFirePath;
        case 79:    return ELF64_MachineTypeZSP;
        case 80:    return ELF64_MachineTypeMMIX;
        case 81:    return ELF64_MachineTypeHUANY;
        case 82:    return ELF64_MachineTypePrism;
        default:    break;
    }
    
    return ELF64_MachineTypeUnknown;
}

const char * ELF64_HeaderGetMachineTypeString( ELF64_HeaderRef header )
{
    switch( ELF64_HeaderGetMachineType( header ) )
    {
        case ELF64_MachineTypeUnknown:      return "Unknown";
        case ELF64_MachineTypeM32:          return "AT&T WE 32100";
        case ELF64_MachineTypeSPARC:        return "SPARC";
        case ELF64_MachineType386:          return "Intel 80386";
        case ELF64_MachineType68K:          return "Motorola 68000";
        case ELF64_MachineType88K:          return "Motorola 88000";
        case ELF64_MachineType860:          return "Intel 80860";
        case ELF64_MachineTypeMIPS:         return "MIPS I Architecture";
        case ELF64_MachineTypeS370:         return "IBM System/370 Processor";
        case ELF64_MachineTypeMIPSRS3LE:    return "MIPS RS3000 Little-endian";
        case ELF64_MachineTypePARISC:       return "Hewlett-Packard PA-RISC";
        case ELF64_MachineTypeVPP500:       return "Fujitsu VPP500";
        case ELF64_MachineTypeSPARC32Plus:  return "Enhanced instruction set SPARC";
        case ELF64_MachineType960:          return "Intel 80960";
        case ELF64_MachineTypePPC:          return "PowerPC";
        case ELF64_MachineTypePPC64:        return "64-bit PowerPC";
        case ELF64_MachineTypeV800:         return "NEC V800";
        case ELF64_MachineTypeFR20:         return "Fujitsu FR20";
        case ELF64_MachineTypeRH32:         return "TRW RH-32";
        case ELF64_MachineTypeRCE:          return "Motorola RCE";
        case ELF64_MachineTypeARM:          return "Advanced RISC Machines ARM";
        case ELF64_MachineTypeAlpha:        return "Digital Alpha";
        case ELF64_MachineTypeSH:           return "Hitachi SH";
        case ELF64_MachineTypeSPARCV9:      return "SPARC Version 9";
        case ELF64_MachineTypeTricore:      return "Siemens Tricore embedded processor";
        case ELF64_MachineTypeARC:          return "Argonaut RISC Core, Argonaut Technologies Inc.";
        case ELF64_MachineTypeH8300:        return "Hitachi H8/300";
        case ELF64_MachineTypeH8300H:       return "Hitachi H8/300H";
        case ELF64_MachineTypeH8S:          return "Hitachi H8S";
        case ELF64_MachineTypeH8500:        return "Hitachi H8/500";
        case ELF64_MachineTypeIA64:         return "Intel IA-64 processor architecture";
        case ELF64_MachineTypeMIPSX:        return "Stanford MIPS-X";
        case ELF64_MachineTypeColdFire:     return "Motorola ColdFire";
        case ELF64_MachineType68HC12:       return "Motorola M68HC12";
        case ELF64_MachineTypeMMA:          return "Fujitsu MMA Multimedia Accelerator";
        case ELF64_MachineTypePCP:          return "Siemens PCP";
        case ELF64_MachineTypeNCPU:         return "Sony nCPU embedded RISC processor";
        case ELF64_MachineTypeNDR1:         return "Denso NDR1 microprocessor";
        case ELF64_MachineTypeStarCore:     return "Motorola Star*Core processor";
        case ELF64_MachineTypeME16:         return "Toyota ME16 processor";
        case ELF64_MachineTypeST100:        return "STMicroelectronics ST100 processor";
        case ELF64_MachineTypeTinyJ:        return "Advanced Logic Corp. TinyJ embedded processor family";
        case ELF64_MachineTypeFX66:         return "Siemens FX66 microcontroller";
        case ELF64_MachineTypeST9Plus:      return "STMicroelectronics ST9+ 8/16 bit microcontroller";
        case ELF64_MachineTypeST7:          return "STMicroelectronics ST7 8-bit microcontroller";
        case ELF64_MachineType68HC16:       return "Motorola MC68HC16 Microcontroller";
        case ELF64_MachineType68HC11:       return "Motorola MC68HC11 Microcontroller";
        case ELF64_MachineType68HC08:       return "Motorola MC68HC08 Microcontroller";
        case ELF64_MachineType68HC05:       return "Motorola MC68HC05 Microcontroller";
        case ELF64_MachineTypeSVX:          return "Silicon Graphics SVx";
        case ELF64_MachineTypeST19:         return "STMicroelectronics ST19 8-bit microcontroller";
        case ELF64_MachineTypeVAX:          return "Digital VAX";
        case ELF64_MachineTypeCRIS:         return "Axis Communications 32-bit embedded processor";
        case ELF64_MachineTypeJavelin:      return "Infineon Technologies 32-bit embedded processor";
        case ELF64_MachineTypeFirePath:     return "Element 14 64-bit DSP Processor";
        case ELF64_MachineTypeZSP:          return "LSI Logic 16-bit DSP Processor";
        case ELF64_MachineTypeMMIX:         return "Donald Knuth's educational 64-bit processor";
        case ELF64_MachineTypeHUANY:        return "Harvard University machine-independent object files";
        case ELF64_MachineTypePrism:        return "SiTera Prism";
    }
}

ELF64_Word ELF64_HeaderGetVersion( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_version;
}

ELF64_Addr ELF64_HeaderGetEntryPointAddress( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_entry;
}

ELF64_Off ELF64_HeaderGetProgramHeaderOffset( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_phoff;
}

ELF64_Off ELF64_HeaderGetSectionHeaderOffset( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_shoff;
}

ELF64_Word ELF64_HeaderGetProcessorSpecificFlags( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_flags;
}

ELF64_Half ELF64_HeaderGetHeaderSize( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_ehsize;
}

ELF64_Half ELF64_HeaderGetProgramHeaderEntrySize( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_phentsize;
}

ELF64_Half ELF64_HeaderGetProgramHeaderEntryCount( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_phnum;
}

ELF64_Half ELF64_HeaderGetSectionHeaderEntrySize( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_shentsize;
}

ELF64_Half ELF64_HeaderGetSectionHeaderEntryCount( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_shnum;
}

ELF64_Half ELF64_HeaderGetSectionNameStringTableIndex( ELF64_HeaderRef header )
{
    if( header == NULL )
    {
        return 0;
    }
    
    return header->e_shstrndx;
}

ELF64_SegmentType ELF64_ProgramHeaderEntryGetType( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return ELF64_SegmentTypeUnused;
    }
    
    switch( entry->p_type )
    {
        case 0:     return ELF64_SegmentTypeUnused;
        case 1:     return ELF64_SegmentTypeLoadable;
        case 2:     return ELF64_SegmentTypeDynamicLinkingTables;
        case 3:     return ELF64_SegmentTypeProgramInterpreterPathName;
        case 4:     return ELF64_SegmentTypeNoteSections;
        case 6:     return ELF64_SegmentTypeProgramHeaderTable;
        default:    break;
    }
    
    return ELF64_SegmentTypeUnused;
}

const char * ELF64_ProgramHeaderEntryGetTypeString( ELF64_ProgramHeaderEntryRef entry )
{
    switch( ELF64_ProgramHeaderEntryGetType( entry ) )
    {
        case ELF64_SegmentTypeUnused:                       return "Unused entry";
        case ELF64_SegmentTypeLoadable:                     return "Loadable segment";
        case ELF64_SegmentTypeDynamicLinkingTables:         return "Dynamic linking tables";
        case ELF64_SegmentTypeProgramInterpreterPathName:   return "Program interpreter path name";
        case ELF64_SegmentTypeNoteSections:                 return "Note sections";
        case ELF64_SegmentTypeProgramHeaderTable:           return "Program header table";
    }
}

ELF64_Word ELF64_ProgramHeaderEntryGetAttributes( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_flags;
}

const char * ELF64_ProgramHeaderEntryGetAttributesString( ELF64_ProgramHeaderEntryRef entry )
{
    ELF64_Word flags;
    
    flags = ELF64_ProgramHeaderEntryGetAttributes( entry );
    
    if( flags & ELF64_SegmentAttributeRead && flags & ELF64_SegmentAttributeWrite && flags & ELF64_SegmentAttributeExecute )
    {
        return "rwx";
    }
    else if( flags & ELF64_SegmentAttributeRead && flags & ELF64_SegmentAttributeWrite )
    {
        return "rw-";
    }
    else if( flags & ELF64_SegmentAttributeRead && flags & ELF64_SegmentAttributeExecute )
    {
        return "r-x";
    }
    else if( flags & ELF64_SegmentAttributeWrite && flags & ELF64_SegmentAttributeExecute )
    {
        return "-wx";
    }
    else if( flags & ELF64_SegmentAttributeRead )
    {
        return "r--";
    }
    else if( flags & ELF64_SegmentAttributeWrite )
    {
        return "-w-";
    }
    else if( flags & ELF64_SegmentAttributeExecute )
    {
        return "--x";
    }
    
    return "---";
}

ELF64_Off ELF64_ProgramHeaderEntryGetFileOffset( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_offset;
}

ELF64_Addr ELF64_ProgramHeaderEntryGetMemoryVirtualAddress( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_vaddr;
}

ELF64_XWord ELF64_ProgramHeaderEntryGetFileSize( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_filesz;
}

ELF64_XWord ELF64_ProgramHeaderEntryGetMemorySize( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_memsz;
}

ELF64_XWord ELF64_ProgramHeaderEntryGetAlignment( ELF64_ProgramHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->p_align;
}

ELF64_Word ELF64_SectionHeaderEntryGetNameOffset( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_name;
}

ELF64_SectionType ELF64_SectionHeaderEntryGetType( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return ELF64_SectionTypeUnused;
    }
    
    switch( entry->sh_type )
    {
        case 0:     return ELF64_SectionTypeUnused;
        case 1:     return ELF64_SectionTypeProgBits;
        case 2:     return ELF64_SectionTypeLinkerSymbolTable;
        case 3:     return ELF64_SectionTypeStringTable;
        case 4:     return ELF64_SectionTypeRelaRelocationEntries;
        case 5:     return ELF64_SectionTypeSymbolHashTable;
        case 6:     return ELF64_SectionTypeDynamicLinkingTables;
        case 7:     return ELF64_SectionTypeNoteInformation;
        case 8:     return ELF64_SectionTypeUninitializedSpace;
        case 9:     return ELF64_SectionTypeRelRelocationEntries;
        case 11:    return ELF64_SectionTypeDynamicLoaderSymbolTable;
        default:    break;
    }
    
    return ELF64_SectionTypeUnused;
}

const char * ELF64_SectionHeaderEntryGetTypeString( ELF64_SectionHeaderEntryRef entry )
{
        switch( ELF64_SectionHeaderEntryGetType( entry ) )
    {
        case        ELF64_SectionTypeUnused:                    return "Unused section";
        case        ELF64_SectionTypeProgBits:                  return "Program information";
        case        ELF64_SectionTypeLinkerSymbolTable:         return "Linker symbol table";
        case        ELF64_SectionTypeStringTable:               return "String table";
        case        ELF64_SectionTypeRelaRelocationEntries:     return "\"Rela\" type relocation entries";
        case        ELF64_SectionTypeSymbolHashTable:           return "Symbol hash table";
        case        ELF64_SectionTypeDynamicLinkingTables:      return "Dynamic linking tables";
        case        ELF64_SectionTypeNoteInformation:           return "Note information";
        case        ELF64_SectionTypeUninitializedSpace:        return "Uninitialized space";
        case        ELF64_SectionTypeRelRelocationEntries:      return "\"Rel\" type relocation entries";
        case        ELF64_SectionTypeDynamicLoaderSymbolTable:  return "Dynamic loader symbol table";
    }
}

ELF64_XWord ELF64_SectionHeaderEntryGetAttributes( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_flags;
}

const char * ELF64_SectionHeaderEntryGetAttributesString( ELF64_SectionHeaderEntryRef entry )
{
    ELF64_XWord flags;
    
    flags = ELF64_SectionHeaderEntryGetAttributes( entry );
    
    if( flags & ELF64_SectionAttributeWrite && flags & ELF64_SectionAttributeAlloc && flags & ELF64_SectionAttributeExecute )
    {
        return "wax";
    }
    else if( flags & ELF64_SectionAttributeWrite && flags & ELF64_SectionAttributeAlloc )
    {
        return "wa-";
    }
    else if( flags & ELF64_SectionAttributeWrite && flags & ELF64_SegmentAttributeExecute )
    {
        return "w-x";
    }
    else if( flags & ELF64_SectionAttributeAlloc && flags & ELF64_SegmentAttributeExecute )
    {
        return "-ax";
    }
    else if( flags & ELF64_SectionAttributeWrite )
    {
        return "w--";
    }
    else if( flags & ELF64_SectionAttributeAlloc )
    {
        return "-a-";
    }
    else if( flags & ELF64_SectionAttributeExecute )
    {
        return "--x";
    }
    
    return "---";
}

ELF64_Addr ELF64_SectionHeaderEntryGetMemoryVirtualAddress( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_addr;
}

ELF64_Off ELF64_SectionHeaderEntryGetFileOffset( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_offset;
}

ELF64_XWord ELF64_SectionHeaderEntryGetSectionSize( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_size;
}

ELF64_Word ELF64_SectionHeaderEntryGetLinkedSectionIndex( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_link;
}

ELF64_Word ELF64_SectionHeaderEntryGetMiscInfo( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_info;
}

ELF64_XWord ELF64_SectionHeaderEntryGetAddressAlignmentBoundary( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_addralign;
}

ELF64_XWord ELF64_SectionHeaderEntryGetEntrySize( ELF64_SectionHeaderEntryRef entry )
{
    if( entry == NULL )
    {
        return 0;
    }
    
    return entry->sh_entsize;
}

unsigned int ELF64_SectionHeaderEntryGetSymbolTableEntryCount( ELF64_SectionHeaderEntryRef section )
{
    ELF64_XWord entrySize;
    ELF64_XWord sectionSize;
    
    if( ELF64_SectionHeaderEntryGetType( section ) != ELF64_SectionTypeLinkerSymbolTable )
    {
        return 0;
    }
    
    entrySize   = ELF64_SectionHeaderEntryGetEntrySize( section );
    sectionSize = ELF64_SectionHeaderEntryGetSectionSize( section );
    
    if( entrySize == 0 )
    {
        return 0;
    }
    
    return ( unsigned int )( sectionSize / entrySize );
}

ELF64_Word ELF64_SymbolTableEntryGetNameOffset( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return 0;
    }
    
    return sym->st_name;
}

ELF64_UChar ELF64_SymbolTableEntryGetInfo( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return 0;
    }
    
    return sym->st_info;
}

ELF64_SymbolType ELF64_SymbolTableEntryGetType( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return ELF64_SymbolTypeNone;
    }
    
    switch( ELF64_SymbolTableEntryGetInfo( sym ) & 0x0F )
    {
        case 0:     return ELF64_SymbolTypeNone;
        case 1:     return ELF64_SymbolTypeDataObject;
        case 2:     return ELF64_SymbolTypeFunctionEntryPoint;
        case 3:     return ELF64_SymbolTypeSection;
        case 4:     return ELF64_SymbolTypeFile;
        default:    break;
    }
    
    return ELF64_SymbolTypeNone;
}

const char * ELF64_SymbolTableEntryGetTypeString( ELF64_SymbolTableEntryRef sym )
{
    switch( ELF64_SymbolTableEntryGetType( sym ) )
    {
        case ELF64_SymbolTypeNone:                  return "No type specified";
        case ELF64_SymbolTypeDataObject:            return "Data object";
        case ELF64_SymbolTypeFunctionEntryPoint:    return "Function entry point";
        case ELF64_SymbolTypeSection:               return "Symbol associated with a section";
        case ELF64_SymbolTypeFile:                  return "Source file associated with the object file";
    }
}

ELF64_SymbolBinding ELF64_SymbolTableEntryGetBinding( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return ELF64_SymbolBindingLocal;
    }
    
    switch( ELF64_SymbolTableEntryGetInfo( sym ) & 0xF0 )
    {
        case 0:     return ELF64_SymbolBindingLocal;
        case 1:     return ELF64_SymbolBindingGlobal;
        case 2:     return ELF64_SymbolBindingWeak;
        default:    break;
    }
    
    return ELF64_SymbolBindingLocal;
}

const char * ELF64_SymbolTableEntryGetBindingString( ELF64_SymbolTableEntryRef sym )
{
    switch( ELF64_SymbolTableEntryGetBinding( sym ) )
    {
        case ELF64_SymbolBindingLocal:  return "Local";
        case ELF64_SymbolBindingGlobal: return "Global";
        case ELF64_SymbolBindingWeak:   return "Weak";
    }
}

ELF64_Half ELF64_SymbolTableEntryGetSectionTableIndex( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return 0;
    }
    
    return sym->st_shndx;
}

ELF64_Addr ELF64_SymbolTableEntryGetSymbolValue( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return 0;
    }
    
    return sym->st_value;
}

ELF64_XWord ELF64_SymbolTableEntryGetObjectSize( ELF64_SymbolTableEntryRef sym )
{
    if( sym == NULL )
    {
        return 0;
    }
    
    return sym->st_size;
}

unsigned long ELF64_Hash( const char * name )
{
    unsigned long h;
    unsigned long g;
    
    h = 0;
    
    while( *( name ) )
    {
        h = ( h << 4 ) + *( ( const unsigned char * )name++ );
        
        if( ( g = h & 0xF0000000 ) )
        {
            h ^= g >> 24;
        }
        
        h &= 0x0FFFFFFF;
    }
    
    return h;
}
