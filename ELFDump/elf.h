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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct __ELF64_File                 * ELF64_FileRef;
typedef struct __ELF64_Header               * ELF64_HeaderRef;
typedef struct __ELF64_ProgramHeaderEntry   * ELF64_ProgramHeaderEntryRef;
typedef struct __ELF64_SectionHeaderEntry   * ELF64_SectionHeaderEntryRef;
typedef struct __ELF64_SymbolTableEntry     * ELF64_SymbolTableEntryRef;

typedef uint64_t    ELF64_Addr;
typedef uint64_t    ELF64_Off;
typedef uint16_t    ELF64_Half;
typedef uint32_t    ELF64_Word;
typedef int32_t     ELF64_SWord;
typedef uint64_t    ELF64_XWord;
typedef int64_t     ELF64_SXWord;
typedef uint8_t     ELF64_UChar;

typedef enum
{
    ELF64_ObjectFileClassUnknown    =   0,
    ELF64_ObjectFileClass32         =   1,
    ELF64_ObjectFileClass64         =   2
}
ELF64_ObjectFileClass;

typedef enum
{
    ELF64_DataEncodingUnknown   =   0,
    ELF64_DataEncodingLSB       =   1,
    ELF64_DataEncodingMSB       =   2
}
ELF64_DataEncoding;

typedef enum
{
    ELF64_OSABIUnknown      =   0,
    ELF64_OSABISysV         =   1,
    ELF64_OSABIHPUX         =   2,
    ELF64_OSABIStandalone   =   255
}
ELF64_OSABI;

typedef enum
{
    ELF64_ObjectFileTypeNone                    =   0,
    ELF64_ObjectFileTypeRelocatableObjectFile   =   1,
    ELF64_ObjectFileTypeExecutableFile          =   2,
    ELF64_ObjectFileTypeSharedObjectFile        =   3,
    ELF64_ObjectFileTypeCoreFile                =   4
}
ELF64_ObjectFileType;

typedef enum
{
    ELF64_MachineTypeUnknown        = 0,    /* No machine */
    ELF64_MachineTypeM32            = 1,    /* AT&T WE 32100 */
    ELF64_MachineTypeSPARC          = 2,    /* SPARC */
    ELF64_MachineType386            = 3,    /* Intel 80386 */
    ELF64_MachineType68K            = 4,    /* Motorola 68000 */
    ELF64_MachineType88K            = 5,    /* Motorola 88000 */
    ELF64_MachineType860            = 7,    /* Intel 80860 */
    ELF64_MachineTypeMIPS           = 8,    /* MIPS I Architecture */
    ELF64_MachineTypeS370           = 9,    /* IBM System/370 Processor */
    ELF64_MachineTypeMIPSRS3LE      = 10,   /* MIPS RS3000 Little-endian */
    ELF64_MachineTypePARISC         = 15,   /* Hewlett-Packard PA-RISC */
    ELF64_MachineTypeVPP500         = 17,   /* Fujitsu VPP500 */
    ELF64_MachineTypeSPARC32Plus    = 18,   /* Enhanced instruction set SPARC */
    ELF64_MachineType960            = 19,   /* Intel 80960 */
    ELF64_MachineTypePPC            = 20,   /* PowerPC */
    ELF64_MachineTypePPC64          = 21,   /* 64-bit PowerPC */
    ELF64_MachineTypeV800           = 36,   /* NEC V800 */
    ELF64_MachineTypeFR20           = 37,   /* Fujitsu FR20 */
    ELF64_MachineTypeRH32           = 38,   /* TRW RH-32 */
    ELF64_MachineTypeRCE            = 39,   /* Motorola RCE */
    ELF64_MachineTypeARM            = 40,   /* Advanced RISC Machines ARM */
    ELF64_MachineTypeAlpha          = 41,   /* Digital Alpha */
    ELF64_MachineTypeSH             = 42,   /* Hitachi SH */
    ELF64_MachineTypeSPARCV9        = 43,   /* SPARC Version 9 */
    ELF64_MachineTypeTricore        = 44,   /* Siemens Tricore embedded processor */
    ELF64_MachineTypeARC            = 45,   /* Argonaut RISC Core, Argonaut Technologies Inc. */
    ELF64_MachineTypeH8300          = 46,   /* Hitachi H8/300 */
    ELF64_MachineTypeH8300H         = 47,   /* Hitachi H8/300H */
    ELF64_MachineTypeH8S            = 48,   /* Hitachi H8S */
    ELF64_MachineTypeH8500          = 49,   /* Hitachi H8/500 */
    ELF64_MachineTypeIA64           = 50,   /* Intel IA-64 processor architecture */
    ELF64_MachineTypeMIPSX          = 51,   /* Stanford MIPS-X */
    ELF64_MachineTypeColdFire       = 52,   /* Motorola ColdFire */
    ELF64_MachineType68HC12         = 53,   /* Motorola M68HC12 */
    ELF64_MachineTypeMMA            = 54,   /* Fujitsu MMA Multimedia Accelerator */
    ELF64_MachineTypePCP            = 55,   /* Siemens PCP */
    ELF64_MachineTypeNCPU           = 56,   /* Sony nCPU embedded RISC processor */
    ELF64_MachineTypeNDR1           = 57,   /* Denso NDR1 microprocessor */
    ELF64_MachineTypeStarCore       = 58,   /* Motorola Star*Core processor */
    ELF64_MachineTypeME16           = 59,   /* Toyota ME16 processor */
    ELF64_MachineTypeST100          = 60,   /* STMicroelectronics ST100 processor */
    ELF64_MachineTypeTinyJ          = 61,   /* Advanced Logic Corp. TinyJ embedded processor family */
    ELF64_MachineTypeFX66           = 66,   /* Siemens FX66 microcontroller */
    ELF64_MachineTypeST9Plus        = 67,   /* STMicroelectronics ST9+ 8/16 bit microcontroller */
    ELF64_MachineTypeST7            = 68,   /* STMicroelectronics ST7 8-bit microcontroller */
    ELF64_MachineType68HC16         = 69,   /* Motorola MC68HC16 Microcontroller */
    ELF64_MachineType68HC11         = 70,   /* Motorola MC68HC11 Microcontroller */
    ELF64_MachineType68HC08         = 71,   /* Motorola MC68HC08 Microcontroller */
    ELF64_MachineType68HC05         = 72,   /* Motorola MC68HC05 Microcontroller */
    ELF64_MachineTypeSVX            = 73,   /* Silicon Graphics SVx */
    ELF64_MachineTypeST19           = 74,   /* STMicroelectronics ST19 8-bit microcontroller */
    ELF64_MachineTypeVAX            = 75,   /* Digital VAX */
    ELF64_MachineTypeCRIS           = 76,   /* Axis Communications 32-bit embedded processor */
    ELF64_MachineTypeJavelin        = 77,   /* Infineon Technologies 32-bit embedded processor */
    ELF64_MachineTypeFirePath       = 78,   /* Element 14 64-bit DSP Processor */
    ELF64_MachineTypeZSP            = 79,   /* LSI Logic 16-bit DSP Processor */
    ELF64_MachineTypeMMIX           = 80,   /* Donald Knuth's educational 64-bit processor */
    ELF64_MachineTypeHUANY          = 81,   /* Harvard University machine-independent object files */
    ELF64_MachineTypePrism          = 82    /* SiTera Prism */
}
ELF64_MachineType;

typedef enum
{
    ELF64_SegmentTypeUnused                     = 0,
    ELF64_SegmentTypeLoadable                   = 1,
    ELF64_SegmentTypeDynamicLinkingTables       = 2,
    ELF64_SegmentTypeProgramInterpreterPathName = 3,
    ELF64_SegmentTypeNoteSections               = 4,
    ELF64_SegmentTypeProgramHeaderTable         = 6
}
ELF64_SegmentType;

typedef enum
{
    ELF64_SegmentAttributeExecute   = 1,
    ELF64_SegmentAttributeWrite     = 2,
    ELF64_SegmentAttributeRead      = 4
}
ELF64_SegmentAttribute;

typedef enum
{
    ELF64_SectionTypeUnused                     = 0,
    ELF64_SectionTypeProgBits                   = 1,
    ELF64_SectionTypeLinkerSymbolTable          = 2,
    ELF64_SectionTypeStringTable                = 3,
    ELF64_SectionTypeRelaRelocationEntries      = 4,
    ELF64_SectionTypeSymbolHashTable            = 5,
    ELF64_SectionTypeDynamicLinkingTables       = 6,
    ELF64_SectionTypeNoteInformation            = 7,
    ELF64_SectionTypeUninitializedSpace         = 8,
    ELF64_SectionTypeRelRelocationEntries       = 9,
    ELF64_SectionTypeDynamicLoaderSymbolTable   = 11
}
ELF64_SectionType;

typedef enum
{
    ELF64_SectionAttributeWrite     = 1,
    ELF64_SectionAttributeAlloc     = 2,
    ELF64_SectionAttributeExecute   = 4
}
ELF64_SectionAttribute;

typedef enum
{
    ELF64_SymbolTypeNone                = 0,
    ELF64_SymbolTypeDataObject          = 1,
    ELF64_SymbolTypeFunctionEntryPoint  = 2,
    ELF64_SymbolTypeSection             = 3,
    ELF64_SymbolTypeFile                = 4
}
ELF64_SymbolType;

typedef enum
{
    ELF64_SymbolBindingLocal    = 0,
    ELF64_SymbolBindingGlobal   = 1,
    ELF64_SymbolBindingWeak     = 2
}
ELF64_SymbolBinding;

ELF64_FileRef               ELF64_ReadFromData( const char * data );
bool                        ELF64_FileIsValid( ELF64_FileRef file );
ELF64_HeaderRef             ELF64_FileGetHeader( ELF64_FileRef file );
ELF64_ProgramHeaderEntryRef ELF64_FileGetProgramHeaderEntry( ELF64_FileRef file, unsigned int index );
ELF64_SectionHeaderEntryRef ELF64_FileGetSectionHeaderEntry( ELF64_FileRef file, unsigned int index );
const char                * ELF64_FileGetNameOfSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section );
const char                * ELF64_FileGetNameOfSectionAtIndex( ELF64_FileRef file, unsigned int index );
ELF64_UChar               * ELF64_FileGetDataForSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section );
ELF64_UChar               * ELF64_FileGetDataForSectionAtIndex( ELF64_FileRef file, unsigned int index );
unsigned int                ELF64_FileGetSymbolTableEntryCountForSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section );
ELF64_SymbolTableEntryRef   ELF64_FileGetSymbolTableEntryForSection( ELF64_FileRef file, ELF64_SectionHeaderEntryRef section, unsigned int index );
const char                * ELF64_FileGetSymbolTableEntryNameForSection( ELF64_FileRef file, ELF64_SymbolTableEntryRef sym, ELF64_SectionHeaderEntryRef section );
ELF64_SectionHeaderEntryRef ELF64_FileGetStringTableHeaderEntry( ELF64_FileRef file );
ELF64_SectionHeaderEntryRef ELF64_FileGetSectionHeaderEntryWithName( ELF64_FileRef file, const char * name );
ELF64_SymbolTableEntryRef   ELF64_FileGetSymbolTableEntryWithName( ELF64_FileRef file, const char * name );
ELF64_SymbolTableEntryRef   ELF64_FileGetSymbolTableEntryWithAddress( ELF64_FileRef file, ELF64_Addr address );

ELF64_ObjectFileClass   ELF64_HeaderGetObjectFileClass( ELF64_HeaderRef header );
const char            * ELF64_HeaderGetObjectFileClassString( ELF64_HeaderRef header );
ELF64_DataEncoding      ELF64_HeaderGetDataEncoding( ELF64_HeaderRef header );
const char            * ELF64_HeaderGetDataEncodingString( ELF64_HeaderRef header );
ELF64_UChar             ELF64_HeaderGetFileVersion( ELF64_HeaderRef header );
ELF64_OSABI             ELF64_HeaderGetOSABI( ELF64_HeaderRef header );
const char            * ELF64_HeaderGetOSABIString( ELF64_HeaderRef header );
ELF64_ObjectFileType    ELF64_HeaderGetObjectFileType( ELF64_HeaderRef header );
const char            * ELF64_HeaderGetObjectFileTypeString( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetMachine( ELF64_HeaderRef header );
ELF64_MachineType       ELF64_HeaderGetMachineType( ELF64_HeaderRef header );
const char *            ELF64_HeaderGetMachineTypeString( ELF64_HeaderRef header );
ELF64_Word              ELF64_HeaderGetVersion( ELF64_HeaderRef header );
ELF64_Addr              ELF64_HeaderGetEntryPointAddress( ELF64_HeaderRef header );
ELF64_Off               ELF64_HeaderGetProgramHeaderOffset( ELF64_HeaderRef header );
ELF64_Off               ELF64_HeaderGetSectionHeaderOffset( ELF64_HeaderRef header );
ELF64_Word              ELF64_HeaderGetProcessorSpecificFlags( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetHeaderSize( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetProgramHeaderEntrySize( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetProgramHeaderEntryCount( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetSectionHeaderEntrySize( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetSectionHeaderEntryCount( ELF64_HeaderRef header );
ELF64_Half              ELF64_HeaderGetSectionNameStringTableIndex( ELF64_HeaderRef header );

ELF64_SegmentType   ELF64_ProgramHeaderEntryGetType( ELF64_ProgramHeaderEntryRef entry );
const char        * ELF64_ProgramHeaderEntryGetTypeString( ELF64_ProgramHeaderEntryRef entry );
ELF64_Word          ELF64_ProgramHeaderEntryGetAttributes( ELF64_ProgramHeaderEntryRef entry );
const char        * ELF64_ProgramHeaderEntryGetAttributesString( ELF64_ProgramHeaderEntryRef entry );
ELF64_Off           ELF64_ProgramHeaderEntryGetFileOffset( ELF64_ProgramHeaderEntryRef entry );
ELF64_Addr          ELF64_ProgramHeaderEntryGetMemoryVirtualAddress( ELF64_ProgramHeaderEntryRef entry );
ELF64_XWord         ELF64_ProgramHeaderEntryGetFileSize( ELF64_ProgramHeaderEntryRef entry );
ELF64_XWord         ELF64_ProgramHeaderEntryGetMemorySize( ELF64_ProgramHeaderEntryRef entry );
ELF64_XWord         ELF64_ProgramHeaderEntryGetAlignment( ELF64_ProgramHeaderEntryRef entry );

ELF64_Word          ELF64_SectionHeaderEntryGetNameOffset( ELF64_SectionHeaderEntryRef entry );
ELF64_SectionType   ELF64_SectionHeaderEntryGetType( ELF64_SectionHeaderEntryRef entry );
const char        * ELF64_SectionHeaderEntryGetTypeString( ELF64_SectionHeaderEntryRef entry );
ELF64_XWord         ELF64_SectionHeaderEntryGetAttributes( ELF64_SectionHeaderEntryRef entry );
const char        * ELF64_SectionHeaderEntryGetAttributesString( ELF64_SectionHeaderEntryRef entry );
ELF64_Addr          ELF64_SectionHeaderEntryGetMemoryVirtualAddress( ELF64_SectionHeaderEntryRef entry );
ELF64_Off           ELF64_SectionHeaderEntryGetFileOffset( ELF64_SectionHeaderEntryRef entry );
ELF64_XWord         ELF64_SectionHeaderEntryGetSectionSize( ELF64_SectionHeaderEntryRef entry );
ELF64_Word          ELF64_SectionHeaderEntryGetLinkedSectionIndex( ELF64_SectionHeaderEntryRef entry );
ELF64_Word          ELF64_SectionHeaderEntryGetMiscInfo( ELF64_SectionHeaderEntryRef entry );
ELF64_XWord         ELF64_SectionHeaderEntryGetAddressAlignmentBoundary( ELF64_SectionHeaderEntryRef entry );
ELF64_XWord         ELF64_SectionHeaderEntryGetEntrySize( ELF64_SectionHeaderEntryRef entry );
unsigned int        ELF64_SectionHeaderEntryGetSymbolTableEntryCount( ELF64_SectionHeaderEntryRef section );

ELF64_Word          ELF64_SymbolTableEntryGetNameOffset( ELF64_SymbolTableEntryRef sym );
ELF64_UChar         ELF64_SymbolTableEntryGetInfo( ELF64_SymbolTableEntryRef sym );
ELF64_SymbolType    ELF64_SymbolTableEntryGetType( ELF64_SymbolTableEntryRef sym );
const char        * ELF64_SymbolTableEntryGetTypeString( ELF64_SymbolTableEntryRef sym );
ELF64_SymbolBinding ELF64_SymbolTableEntryGetBinding( ELF64_SymbolTableEntryRef sym );
const char        * ELF64_SymbolTableEntryGetBindingString( ELF64_SymbolTableEntryRef sym );
ELF64_Half          ELF64_SymbolTableEntryGetSectionTableIndex( ELF64_SymbolTableEntryRef sym );
ELF64_Addr          ELF64_SymbolTableEntryGetSymbolValue( ELF64_SymbolTableEntryRef sym );
ELF64_XWord         ELF64_SymbolTableEntryGetObjectSize( ELF64_SymbolTableEntryRef sym );

unsigned long ELF64_Hash( const char * name );
