# GTA V Wardrobe Analyzer - Usage Guide

This comprehensive guide will walk you through using the GTA V Wardrobe Analyzer to extract and analyze clothing data from Grand Theft Auto V.

## 📋 Table of Contents

1. [Prerequisites & Setup](#prerequisites--setup)
2. [Data Preparation](#data-preparation)
3. [Running the Tool](#running-the-tool)
4. [Understanding Output](#understanding-output)
5. [Troubleshooting](#troubleshooting)
6. [Advanced Usage](#advanced-usage)
7. [FAQ](#faq)

## Prerequisites & Setup

### System Requirements
- **OS**: Windows 10/11 (64-bit)
- **RAM**: 4GB minimum, 8GB recommended
- **Storage**: 1GB free space for output files
- **CPU**: Any modern x64 processor

### Software Requirements
- **Visual Studio 2019 or later** with C++17 support
- **GTA V game files** (legally owned copy)
- **Git** (for cloning the repository)

### Installation Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/yourusername/GenderSwapDump.git
   cd GenderSwapDump
   ```

2. **Open in Visual Studio**
   - Open `GenderSwapDump.sln`
   - Ensure platform is set to `x64`
   - Choose `Release` configuration for best performance

3. **Build the Project**
   - Press `Ctrl+Shift+B` or go to Build → Build Solution
   - The executable will be created in `x64/Release/GenderSwapDump.exe`

## Data Preparation

### Required Files

#### 1. XML Files (Clothing Definitions)
**Location**: `GenderSwapDump/xmlfiles/`

These are the core clothing definition files extracted from GTA V. You need both male and female versions:

**File Naming Convention**:
```
mp_m_freemode_01_mp_m_[dlc]_01.xml  # Male clothing
mp_f_freemode_01_mp_f_[dlc]_01.xml  # Female clothing
```

**Examples**:
```
mp_m_freemode_01_mp_m_beach_01.xml      # Beach Bum (Male)
mp_f_freemode_01_mp_f_beach_01.xml      # Beach Bum (Female)
mp_m_freemode_01_mp_m_business_01.xml   # Business (Male)
mp_f_freemode_01_mp_f_business_01.xml   # Business (Female)
```

**How to Extract**:
1. Use OpenIV or similar tool to extract from GTA V
2. Navigate to `x64v.rpf/models/cdimages/streamedpeds_mp.rpf/mp_[gender]_freemode_01`
3. Look for XML files matching the pattern above

#### 2. JSON Files (Localization Data)
**Location**: `GenderSwapDump/Jsonfiles/`

Download from the [v-clothingnames repository](https://github.com/root-cause/v-clothingnames):

**Required Files**:
```
male_tops.json          # Male torso items
female_tops.json        # Female torso items
male_legs.json          # Male leg items
female_legs.json        # Female leg items
male_shoes.json         # Male footwear
female_shoes.json       # Female footwear
male_glasses.json       # Male eyewear
female_glasses.json     # Female eyewear
props_male_*.json       # Male props/accessories
props_female_*.json     # Female props/accessories
```

#### 3. Script Metadata File
**Location**: Root directory (`scriptmetadata.meta`)

This file contains the script metadata used for gender swap detection. Extract this from GTA V files or use a provided sample.

### Folder Structure Check
Before running, ensure your structure looks like this:
```
GenderSwapDump/
├── GenderSwapDump/
│   ├── xmlfiles/
│   │   ├── mp_m_freemode_01_mp_m_beach_01.xml
│   │   ├── mp_f_freemode_01_mp_f_beach_01.xml
│   │   └── [more XML files...]
│   ├── Jsonfiles/
│   │   ├── male_tops.json
│   │   ├── female_tops.json
│   │   └── [more JSON files...]
│   └── x64/Release/GenderSwapDump.exe
├── scriptmetadata.meta
└── output/ (will be created automatically)
```

## Running the Tool

### Basic Usage

1. **Open Command Prompt or PowerShell**
   ```bash
   cd path/to/GenderSwapDump
   ```

2. **Run the Executable**
   ```bash
   ./x64/Release/GenderSwapDump.exe
   ```

3. **Monitor Progress**
   The tool will display progress information:
   ```
   Starting GTA V Wardrobe Analysis...
   Loading XML files from xmlfiles/...
   Found 47 XML files to process
   Loading JSON localization data...
   Processing script metadata...
   
   Processing Items:
   ████████████████████████████████████████ 100% (2,847 items)
   
   Generating output files...
   Analysis complete! Check the output/ directory.
   ```

### Output Generation

The tool automatically creates an `output/` directory with several files:

#### Main Files
- **`clothing_dump.txt`**: Complete analysis of all items
- **`processing_log.txt`**: Detailed processing information and any errors

#### DLC-Specific Files
- **`items_mpbeach.txt`**: Beach Bum Update items
- **`items_mpbusiness.txt`**: Business Update items
- **`items_mpheist.txt`**: Heists Update items
- **`outfits_mpheist3.txt`**: Diamond Casino Heist outfits
- **`[and many more...]`**

#### Component-Specific Files
- **`items_TORSO.txt`**: All torso items across DLCs
- **`items_LEGS.txt`**: All leg items across DLCs
- **`items_SHOES.txt`**: All footwear across DLCs
- **`items_OUTFIT.txt`**: Complete outfit sets

## Understanding Output

### Main Dump File Format

The `clothing_dump.txt` file is organized by gender:

```
MALE COLLECTION:
================================================================================

Item: Classic Suit
  Key: DLC_MP_H3_M_OUTFIT_SL_31
  Gender: Male
  Type: OUTFIT
  DLC: The Diamond Casino Heist (mpheist3)
  Drawable ID: 31
  Texture ID: 0
  Unique Hash: DLC_MP_H3_M_OUTFIT_SL_31_0
  Script Value: 12345

Item: Beach Casual Shirt
  Key: DLC_MP_BEACH_M_TORSO_15_3
  Gender: Male
  Type: TORSO
  DLC: Beach Bum Update (mpbeach)
  Drawable ID: 15
  Texture ID: 3
  Unique Hash: DLC_MP_BEACH_M_TORSO_15_3
  Text Label: "TORSO_P1_15_3" = "Beach Casual Shirt"

================================================================================

FEMALE COLLECTION:
================================================================================

[Female items follow same format...]
```

### Field Explanations

- **Item**: Human-readable name (from JSON files or auto-generated)
- **Key**: Internal unique identifier
- **Gender**: Male or Female
- **Type**: Component type (TORSO, LEGS, SHOES, OUTFIT, etc.)
- **DLC**: Resolved DLC name and pack identifier
- **Drawable ID**: Primary asset ID for modding
- **Texture ID**: Texture variation ID
- **Unique Hash**: Raw hash from game files
- **Script Value**: Value used for gender swap detection
- **Text Label**: Localization key and translated name

### DLC Resolution Examples

The tool intelligently extracts DLC information from hash patterns:

```
Hash: DLC_MP_JAN_M_OUTFIT_0
├── Extracted Key: DLC_MP_JAN
├── Resolved Name: "January 2016 Update"
└── Pack ID: mpjanuary2016

Hash: DLC_MP_BEACH_F_TORSO_5_2  
├── Extracted Key: DLC_MP_BEACH
├── Resolved Name: "Beach Bum Update"
└── Pack ID: mpbeach

Hash: DLC_MP_SUM24_G9EC_F_LEGS_12_0
├── Extracted Key: DLC_MP_SUM24_G9EC
├── Resolved Name: "Summer 2024 G9EC"
└── Pack ID: mpsum24_g9ec
```

### Component Type Detection

The tool automatically detects component types from various sources:

1. **Hash Patterns**: `DLC_MP_BEACH_M_TORSO_15_3` → TORSO
2. **XML Data**: `<Type>PED_VARIATION_TORSO</Type>` → TORSO
3. **File Context**: Found in `male_tops.json` → TORSO
4. **Fallback**: If detection fails → UNKNOWN

## Advanced Usage

### Processing Multiple DLC Packs

To process specific DLC packs only:

1. **Place only desired XML files** in `xmlfiles/` directory
2. **Run the tool** - it will automatically detect which DLCs to process
3. **Check output** - only specified DLCs will appear in results

### Custom JSON Data

If you have custom localization data:

1. **Create JSON files** following the same format as v-clothingnames
2. **Place in `Jsonfiles/`** directory
3. **Use standard naming**: `[gender]_[component].json`

Example custom JSON structure:
```json
{
  "DLC_MP_CUSTOM_M_TORSO_0_0": {
    "nameHash": "TORSO_P1_CUSTOM_0_0",
    "name": "Custom Shirt",
    "localized": "My Custom Shirt"
  }
}
```

### Debug Mode

For detailed troubleshooting, use the Debug build:

1. **Build in Debug configuration**
2. **Run**: `./x64/Debug/GenderSwapDump.exe`
3. **Check**: `output/debug_log.txt` for verbose information

Debug output includes:
- File loading details
- Hash extraction process
- DLC resolution steps
- Component type detection logic
- Error details and stack traces

## Troubleshooting

### Common Issues

#### "No XML files found"
**Cause**: XML files not in correct location or wrong naming
**Solution**: 
- Verify files are in `GenderSwapDump/xmlfiles/`
- Check naming: `mp_[m/f]_freemode_01_mp_[m/f]_[dlc]_01.xml`
- Ensure files are valid XML (not corrupted)

#### "Script metadata not found"
**Cause**: Missing `scriptmetadata.meta` file
**Solution**:
- Place file in root directory (same level as GenderSwapDump folder)
- Extract from GTA V files or use provided sample
- Check file is not empty or corrupted

#### "No items processed"
**Cause**: XML files don't contain expected data structure
**Solution**:
- Verify XML files are from correct GTA V version
- Check files contain `<Item>` elements with required fields
- Try with known working XML files first

#### "DLC names showing as raw keys"
**Cause**: Unknown DLC not in mapping table
**Solution**:
- Check `Core/Constants.h` for DLC mappings
- Add new DLC to `DLC_LIST` array
- Submit issue on GitHub with DLC details

#### "Component types showing as UNKNOWN"
**Cause**: Hash pattern not recognized or new component type
**Solution**:
- This is normal for some items
- Check if pattern should be added to `extractComponentType()`
- Items still process correctly, just lack specific type

### Performance Issues

#### Slow Processing
**Cause**: Large dataset or insufficient RAM
**Solutions**:
- Close other applications to free RAM
- Process smaller batches of XML files
- Use Release build instead of Debug

#### Large Output Files
**Cause**: Extensive clothing database
**Solutions**:
- Output is normal for complete GTA V database
- Use DLC-specific files for targeted data
- Compress output directory for storage

### Memory Issues

#### "Out of Memory" Errors
**Cause**: Insufficient system RAM
**Solutions**:
- Ensure 4GB+ available RAM
- Process smaller batches
- Use 64-bit build (should be default)

## FAQ

### General Questions

**Q: What GTA V versions are supported?**
A: All versions with DLC content. The tool adapts to available XML files.

**Q: Can I use this with FiveM/modified game files?**
A: Yes, as long as XML structure remains compatible.

**Q: How often should I update JSON files?**
A: Check v-clothingnames repository for updates when new GTA V updates are released.

### Technical Questions

**Q: Why are some items missing localized names?**
A: Not all items have entries in JSON files. The tool generates readable names automatically.

**Q: What's the difference between Drawable ID and Texture ID?**
A: Drawable ID is the main asset, Texture ID is the color/pattern variation.

**Q: Can I extract data for specific characters?**
A: This tool focuses on freemode characters. Other characters require different XML files.

### Output Questions

**Q: Why are there separate DLC files?**
A: For easier navigation and modding purposes. Each DLC can be reviewed independently.

**Q: What's the purpose of the Script Value?**
A: Used for gender swap detection - items with same value but different genders can be swapped.

**Q: Can I import this data into other tools?**
A: Yes, output is human-readable text format that can be parsed by other applications.

## Getting Help

If you encounter issues not covered in this guide:

1. **Check the GitHub Issues** page for similar problems
2. **Create a new issue** with:
   - Your system specifications
   - Error messages (copy full text)
   - Steps to reproduce the problem
   - Sample files if relevant

3. **Join the discussion** on GitHub Discussions for general questions

## Contributing

To improve this guide or the tool:

1. **Fork the repository**
2. **Make improvements** to documentation or code
3. **Submit a pull request** with clear description of changes

Your contributions help make this tool better for the entire GTA V modding community!

---

**Last Updated**: December 2024  
**Tool Version**: v2.1.0  
**Guide Version**: v1.0.0 