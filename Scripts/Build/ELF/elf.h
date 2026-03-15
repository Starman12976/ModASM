/**
 *  @file   elf.h
 *  @brief  TODO
 *  @author Joel Nelems
 *  @date   2026-3-15
 *
 *  @details
 *  TODO
 *
 *  @example
 *  TODO
 */

// Include guards
#ifndef ELF_H
#define ELF_H

// Include libraries
#include <stdint.h>

/**
 *  @enum  EIMagCode
 *  @brief Represents the file signature of an ELF file.
 */
typedef enum : uint8_t {
    EI_MAG0 = 0x7F,
    EI_MAG1 = 0x45,
    EI_MAG2 = 0x4C,
    EI_MAG3 = 0x46
} EIMagCode;

/**
 *  @enum  EIClass
 *  @brief Represents the word size of the architecture.
 */
typedef enum : uint8_t {
    EI_CLASS_32 = 0x01,
    EI_CLASS_64 = 0x02
} EIClass;

/**
 *  @enum  EIData
 *  @brief Represents the most significant bit of the architecture.
 */
typedef enum : uint8_t {
    EI_DATA_LE = 0x01,
    EI_DATA_BE = 0x02
} EIData;

/**
 *  @enum  EIVersion
 *  @brief Represents the ELF version.
 */
typedef enum : uint8_t {
    EI_VERSION_CURRENT = 1
} EIVersion;

/**
 *  @enum  EIOSABI
 *  @brief Represents the operating system's ABI.
 */
typedef enum : uint8_t {
    EI_OSABI_SYSTEM_V          = 0x00,
    EI_OSABI_HP_UX             = 0x01,
    EI_OSABI_HP_NETBSD         = 0x02,
    EI_OSABI_HP_LINUX          = 0x03,
    EI_OSABI_HP_GNU_HURD       = 0x04,
    EI_OSABI_HP_SOLARIS        = 0x06,
    EI_OSABI_HP_AIX            = 0x07,
    EI_OSABI_HP_IRIX           = 0x08,
    EI_OSABI_HP_FREEBSD        = 0x09,
    EI_OSABI_HP_TRU64          = 0x0A,
    EI_OSABI_HP_NOVELL_MODESTO = 0x0B,
    EI_OSABI_HP_OPENBSD        = 0x0C,
    EI_OSABI_HP_OPENVMS        = 0x0D,
    EI_OSABI_HP_NONSTOP_KERNEL = 0x0E,
    EI_OSABI_HP_AROS           = 0x0F,
    EI_OSABI_HP_FENIXOS        = 0x10,
    EI_OSABI_HP_NUXI_CLOUDABI  = 0x11,
    EI_OSABI_HP_OPENVOS        = 0x12,
} EIOSABI;

/**
 *  @enum  EIABIVersion
 *  @brief Represents the ABI version.
 */
typedef enum : uint8_t {
    EI_ABI_VERSION_NONE = 0x00
} EIABIVersion;

/**
 *  @enum  EIPadding
 *  @brief Represents currently unused bits. Reserved in case of changes.
 */
typedef enum : uint8_t {
    EI_PAD0,
    EI_PAD1,
    EI_PAD2,
    EI_PAD3,
    EI_PAD4,
    EI_PAD5,
    EI_PAD6
} EIPadding;

/**
 *  @struct EIdent
 *  @brief  Contains file data, including data specifications and architecture.
 */
typedef struct __attribute__((packed)) {
    EIMagCode     MAG[4];
    EIClass       CLASS;
    EIData        DATA;
    EIVersion     VERSION;
    EIOSABI       OSABI;
    EIVersion     ABIVERSION;
    EIPadding     EI_PAD[7];
} EIdent;

/**
 *  @enum  EType
 *  @brief Represents the purpose of the file.
 */
typedef enum : uint16_t {
    ET_NONE   = 0x00,
    ET_REL    = 0x01,
    ET_EXEC   = 0x02,
    ET_DYN    = 0x03,
    ET_CORE   = 0x04,
    ET_LOOS   = 0xFE00,
    ET_HIOS   = 0xFEFF,
    ET_LOPROC = 0xFF00,
    ET_HIPROC = 0xFFFF
} EType;

/**
 *  @enum  EMachine
 *  @brief Represents the instruction set of the file.
 */
typedef enum : uint16_t {
    EM_NONE              = 0x00,
    EM_WE_32100          = 0x01,
    EM_SPARC             = 0x02,
    EM_X86               = 0x03,
    EM_M68K              = 0x04,
    EM_M88K              = 0x05,
    EM_INTEL_MCU         = 0x06,
    EM_MIPS              = 0x08,
    EM_IBM_SYSTEM        = 0x09,
    EM_RS3000            = 0x0A,
    EM_PA_RISC           = 0x0F,
    EM_INTEL_80960       = 0x13,
    EM_POWERPC           = 0x14,
    EM_POWERPC_64        = 0x15,
    EM_S390              = 0x16,
    EM_IBM_SPU           = 0x17,
    EM_NEC_V800          = 0x24,
    EM_FUJITSU_FR20      = 0x25,
    EM_TRW_RH_32         = 0x26,
    EM_MOTOROLA_RCE      = 0x27,
    EM_ARM               = 0x28,
    EM_DIGITAL_ALPHA     = 0x29,
    EM_SUPERH            = 0x2A,
    EM_SPARC_V9          = 0x2B,
    EM_SIEMENS_TRICORE   = 0x2C,
    EM_ARGONAUT_RISC     = 0x2D,
    EM_HITACHI_H8_300    = 0x2E,
    EM_HITACHI_H8_300H   = 0x2F,
    EM_HITACHI_H8S       = 0x30,
    EM_HITACHI_H8_500    = 0x31,
    EM_IA_64             = 0x32,
    EM_STANFORD_MIPS     = 0x33,
    EM_MOTOROLA_COLDFIRE = 0x34,
    EM_MOTOROLA_M68      = 0x35,
    EM_FUJITSU_MMA       = 0x36,
    EM_SIEMENS_PCP       = 0x37,
    EM_SONY_NCPU         = 0x38,
    EM_DENSO_NDR1        = 0x39,
    EM_MOTOROLA_STAR     = 0x3A,
    EM_TOYOTA_ME16       = 0x3B,
    EM_ST100             = 0x3C,
    EM_ALC_TINYJ         = 0x3D,
    EM_AMD_X86_64        = 0x3E,
    EM_SONY_DSP          = 0x3F,
    EM_DEC_PDP_10        = 0x40,
    EM_DEC_PDP_11        = 0x41,
    EM_SIEMENS_FX66      = 0x42,
    EM_STM_ST9           = 0x43,
    EM_STM_ST7           = 0x44,
    EM_MOTOROLA_HC16     = 0x45,
    EM_MOTOROLA_HC11     = 0x46,
    EM_MOTOROLA_HC08     = 0x47,
    EM_MOTOROLA_HC05     = 0x48,
    EM_SILICON_SVX       = 0x49,
    EM_STM_ST19          = 0x4A,
    EM_DIGITAL_VAX       = 0x4B,
    EM_AXIS_COMM_32      = 0x4C,
    EM_INFIN_32          = 0x4D,
    EM_ELEM_14           = 0x4E,
    EM_LSI_LOGIC         = 0x4F,
    EM_TMS320            = 0x8C,
    EM_ELBRUS_E2K        = 0xAF,
    EM_AARCH_64          = 0xB7,
    EM_ZILOG_Z80         = 0xDC,
    EM_RISC_V            = 0xF3,
    EM_BERKELEY_PACKET   = 0xF7,
    EM_WDC_65C816        = 0x101,
    EM_LOONG_ARCH        = 0x102
} EMachine;

/**
 *  @enum  EVersion
 *  @brief Represents the ELF version.
 */
typedef enum : uint32_t {
    EV_CURRENT = 1
} EVersion;

/**
 *  @struct ELF64Header
 *  @brief  Identifies file type and architecture.
 */
typedef struct __attribute__((packed)) {
    // Declare ELF64Header members
    EIdent   ident;
    EType    type;
    EMachine e_machine;
    EVersion e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ELF64Header;

/**
 *  @enum  PType
 *  @brief Represents the segment type.
 */
typedef enum : uint32_t {
    PT_NULL    = 0x00000000,
    PT_LOAD    = 0x00000001,
    PT_DYNAMIC = 0x00000002,
    PT_INTERP  = 0x00000003,
    PT_NOTE    = 0x00000004,
    PT_SHLIB   = 0x00000005,
    PT_PHDR    = 0x00000006,
    PT_TLS     = 0x00000007,
    PT_LOOS    = 0x60000000,
    PT_HIOS    = 0x6FFFFFFF,
    PT_LOPROC  = 0x70000000,
    PT_HIPROC  = 0x7FFFFFFF
} PType;

/**
 *  @enum  PFlags
 *  @brief Represents segment-dependent flags.
 */
typedef enum : uint32_t {
    PF_X = 0x1,
    PF_W = 0x2,
    PF_R = 0x4
} PFlags;

/**
 *  @struct ELF64ProgramHeader
 *  @brief  Tells the system how to create a process image.
 */
typedef struct __attribute__((packed)) {
    // Declare ELF64ProgramHeader members
    PType    p_type;
    PFlags   p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} ELF64ProgramHeader;

/**
 *  @enum  SHType
 *  @brief Represents the header type.
 */
typedef enum : uint32_t {
    SHT_NULL          = 0x0,
    SHT_PROGBITS      = 0x1,
    SHT_SYMTAB        = 0x2,
    SHT_STRTAB        = 0x3,
    SHT_RELA          = 0x4,
    SHT_HASH          = 0x5,
    SHT_DYNAMIC       = 0x6,
    SHT_NOTE          = 0x7,
    SHT_NOBITS        = 0x8,
    SHT_REL           = 0x9,
    SHT_SHLIB         = 0xA,
    SHT_DYNSYM        = 0xB,
    SHT_INIT_ARRAY    = 0xE,
    SHT_FINI_ARRAY    = 0xF,
    SHT_PREINIT_ARRAY = 0x10,
    SHT_GROUP         = 0x11,
    SHT_SYMTAB_SHNDX  = 0x12,
    SHT_NUM           = 0x13,
    SHT_LOOS          = 0x60000000
} SHType;

/**
 *  @enum  SHFlags
 *  @brief Represents section attributes.
 */
typedef enum : uint64_t {
    SHF_WRITE            = 0x1,
    SHF_ALLOC            = 0x2,
    SHF_EXECINSTR        = 0x4,
    SHF_MERGE            = 0x10,
    SHF_STRINGS          = 0x20,
    SHF_INFO_LINK        = 0x40,
    SHF_LINK_ORDER       = 0x80,
    SHF_OS_NONCONFORMING = 0x100,
    SHF_GROUP            = 0x200,
    SHF_TLS              = 0x400,
    SHF_MASKOS           = 0x0FF00000,
    SHF_MASKPROC         = 0xF0000000,
    SHF_ORDERED          = 0x40000000,
    SHF_EXCLUDE          = 0x80000000
} SHFlags;

/**
 *  @struct ELF64SectionHeader
 *  @brief  Contains code section data.
 */
typedef struct __attribute__((packed)) {
    uint32_t sh_name;
    SHType   sh_type;
    SHFlags  sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} ELF64SectionHeader;

/**
 *  @struct ELF64Section
 *  @brief  Stores section information and data.
 */
typedef struct {
    ELF64SectionHeader shdr;
    uint8_t           *data;
    uint64_t           capacity;
} ELF64Section;

/**
 *  @struct ELF64Segment
 *  @brief  Stores program information and data.
 */
typedef struct {
    ELF64ProgramHeader phdr;
    uint8_t           *data;
    uint64_t           capacity;
} ELF64Segment;

/**
 *  @struct ELF64
 *  @brief  Represents a 64-bit ELF file.
 */
typedef struct {
    // Header
    ELF64Header         ehdr;

    // Sections
    ELF64SectionHeader *shdrs;
    size_t              s_count;
    size_t              s_cap;

    // Segments
    ELF64ProgramHeader *phdrs;
    size_t              p_count;
    size_t              p_cap;
} ELF64;

// Include guard
#endif //ELF_H
