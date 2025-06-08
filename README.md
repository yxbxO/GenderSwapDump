# GTA V Wardrobe Analyzer

A comprehensive GTA V clothing data extraction and analysis tool that processes game files to identify gender swap clothing pairs and extract detailed component information.

## 🎯 Overview

GTA V Wardrobe Analyzer is a specialized tool designed to analyze Grand Theft Auto V clothing data files and extract meaningful information about:
- **Gender Swap Pairs**: Clothing items that can be swapped between male and female characters
- **Component Details**: Detailed information about clothing components, textures, and variations
- **DLC Integration**: Intelligent DLC detection from `uniqueNameHash` with support for 46+ DLC packs
- **Outfit Processing**: Complete outfit analysis with component relationships

## ✨ Features

### Core Functionality
- **Intelligent Gender Matching**: Identifies clothing pairs that can be gender-swapped based on script metadata values
- **Enhanced DLC Resolution**: Smart extraction of DLC information from `uniqueNameHash` patterns like `DLC_MP_JAN_M_OUTFIT_0`
- **Component Type Detection**: Automatic detection of component types from hash patterns and XML data
- **Unified Processing**: Streamlined processing pipeline with efficient caching and memory usage
- **Localization Support**: Extracts text labels and localized names from game files

### Data Processing
- **Hash-Based DLC Detection**: Extracts DLC keys directly from `uniqueNameHash` for reliable identification
- **Smart Caching**: Efficient caching system for faster processing
- **Error Handling**: Robust error handling with detailed logging
- **Performance Optimized**: Streamlined codebase with redundant code removed

### Output Formats
- **Detailed Reports**: Human-readable reports with comprehensive item information
- **DLC-Specific Dumps**: Separate files for each DLC pack (e.g., `items_mpbeach.txt`)
- **Component-Specific Dumps**: Items organized by component type (e.g., `items_TORSO.txt`)
- **Outfit Analysis**: Complete outfit breakdowns with component relationships

## 🚀 Quick Start

### Prerequisites
- Windows 10/11
- Visual Studio 2019 or later (with C++17 support)
- GTA V game files access

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/GenderSwapDump.git
   cd GenderSwapDump
   ```

2. **Open the solution**:
   ```bash
   # Open in Visual Studio
   start GenderSwapDump.sln
   ```

3. **Build the project**:
   - Set configuration to `Release` or `Debug`
   - Build the solution (Ctrl+Shift+B)

### Setup Data Files

1. **XML Files**: Place GTA V clothing XML files in `GenderSwapDump/xmlfiles/`
   - `mp_m_freemode_01_mp_m_[dlc]_01.xml` (male clothing)
   - `mp_f_freemode_01_mp_f_[dlc]_01.xml` (female clothing)

2. **JSON Files**: Place component JSON files in `GenderSwapDump/Jsonfiles/`
   - Download from [v-clothingnames repository](https://github.com/root-cause/v-clothingnames)
   - Examples: `male_tops.json`, `female_legs.json`, etc.

3. **Script Metadata**: Ensure `scriptmetadata.meta` is in the root directory

### Basic Usage

```bash
# Run the executable
./x64/Release/GenderSwapDump.exe

# Output files will be generated in ./output/:
# - clothing_dump.txt: Complete clothing analysis
# - items_[dlc].txt: Items by DLC (e.g., items_mpbeach.txt)
# - outfits_[dlc].txt: Outfits by DLC
# - items_[component].txt: Items by component type
```

## 📁 Project Structure

```
GenderSwapDump/
├── GenderSwapDump/
│   ├── Core/                    # Core functionality and constants
│   │   ├── Constants.h          # DLC mappings, component types, utilities
│   │   └── Interfaces.h         # Interface definitions
│   ├── Models/                  # Data models and structures
│   │   └── ClothingTypes.h      # Clothing item and component definitions
│   ├── Parsers/                 # File parsing functionality
│   │   ├── XmlParser.cpp/h      # XML file parsing
│   │   ├── JsonParser.cpp/h     # JSON file parsing
│   │   └── ScriptMetadataParser.cpp/h  # Script metadata parsing
│   ├── Processors/              # Data processing logic
│   │   └── ClothingProcessor.cpp/h  # Main processing engine
│   ├── Output/                  # Output generation
│   │   └── ClothingDumper.cpp/h # Report generation and formatting
│   ├── xmlfiles/               # Input XML files directory
│   ├── Jsonfiles/              # Input JSON files directory
│   └── Source.cpp              # Main entry point
├── Dependencies/               # External dependencies
├── output/                     # Generated output files
├── README.md                  # This file
├── USAGE_GUIDE.md             # Detailed usage instructions
└── DLC_UPDATE_GUIDE.md        # Guide for adding new DLC support
```

## 🔧 Configuration

### Supported File Types
- **XML Files**: GTA V clothing component definitions from game files
- **JSON Files**: Component metadata and localization data
- **META Files**: Script metadata for value matching

### DLC Support
The tool supports **46+ GTA V DLC packs** with intelligent detection:

#### Hash-Based Detection
The tool extracts DLC information directly from `uniqueNameHash` patterns:
```
DLC_MP_JAN_M_OUTFIT_0     → January 2016 Update
DLC_MP_BEACH_F_TORSO_5_2  → Beach Bum Update  
DLC_MP_H3_M_BERD_9_0      → The Diamond Casino Heist
DLC_MP_SUM24_G9EC_F_LEGS_12_0 → Summer 2024 G9EC
```

#### Supported DLC Packs
- **Beach Bum Update** (mpbeach)
- **Business Update** (mpbusiness, mpbusiness2)
- **Heists Update** (mpheist)
- **Diamond Casino Heist** (mpheist3)
- **Cayo Perico Heist** (mpheist4)
- **The Contract** (mpsecurity)
- **Summer/Winter Updates** (mpSum2, mpchristmas3, mp2023_01, mp2024_01, etc.)
- And many more...

### Component Types
Recognizes all standard GTA V component types:
- **TORSO**: Upper body clothing, jackets, shirts
- **LEGS**: Lower body clothing, pants, skirts  
- **SHOES**: Footwear, boots, sneakers
- **GLASSES**: Eyewear, sunglasses
- **MASK**: Face accessories, masks
- **HAIR**: Hairstyles
- **GLOVES**: Hand accessories
- **OUTFIT**: Complete outfit sets
- **And more...**

## 📊 Output Explanation

### Main Dump File (`clothing_dump.txt`)
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
```

### DLC-Specific Files (`items_mpbeach.txt`)
```
ITEMS BY DLC
================================================================================

Item: Beach Casual Shirt
  Key: DLC_MP_BEACH_M_TORSO_15_3
  Gender: Male
  Type: TORSO
  DLC: Beach Bum Update (mpbeach)
  Drawable ID: 15
  Texture ID: 3
```

### Component-Specific Files (`items_TORSO.txt`)
All torso items across all DLCs, organized for easy browsing.

### Technical Details
Each item includes:
- **Unique Hash**: Internal game identifier
- **DLC Information**: Resolved from hash patterns
- **Component Type**: Automatically detected
- **Drawable/Texture IDs**: For game modding
- **Localized Names**: Human-readable names when available

## 🛠️ Advanced Usage

### Performance Tuning
- **Memory Usage**: ~100-300MB depending on dataset size
- **Processing Time**: 5-15 seconds for full GTA V clothing database
- **Output Size**: 10-100MB depending on data complexity

### Hash-Based DLC Resolution
The tool now intelligently extracts DLC information from `uniqueNameHash`:
```cpp
// Examples of hash patterns:
"DLC_MP_JAN_M_OUTFIT_0"      → "January 2016 Update"
"DLC_MP_BEACH_F_TORSO_5_2"   → "Beach Bum Update"  
"DLC_MP_H3_M_BERD_9_0"       → "The Diamond Casino Heist"
```

This approach is more reliable than parsing XML `dlcName` fields like `"Male_freemode_beach"`.

## 🐛 Troubleshooting

### Common Issues

**1. "Failed to load XML files"**
- Ensure XML files are placed in `GenderSwapDump/xmlfiles/`
- Check file permissions and encoding (should be UTF-8)
- Verify file naming: `mp_[gender]_freemode_01_mp_[gender]_[dlc]_01.xml`

**2. "No items found"**
- Verify `scriptmetadata.meta` is present and accessible
- Check that both male and female clothing files are available
- Ensure JSON files from v-clothingnames are up to date

**3. "Component type detection failed"**
- This is normal for some items - they'll be marked as "UNKNOWN"
- The tool handles this gracefully and continues processing

**4. "DLC names showing as raw values"**
- Update `DLC_LIST` in `Core/Constants.h` with missing DLC mappings
- The tool should auto-detect most DLCs from hash patterns

### Debug Mode
Run with debug configuration for detailed logging:
```bash
./x64/Debug/GenderSwapDump.exe
```

Check `output/clothing_dump.log` for detailed processing information.

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes** with proper documentation
4. **Submit a pull request**

### Adding New DLC Support
See `DLC_UPDATE_GUIDE.md` for detailed instructions on adding new DLC packs.

Quick steps:
1. Update `DLC_LIST` in `Core/Constants.h`
2. Add proper display name and filename mapping
3. Test with sample files from the new DLC

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Rockstar Games** for creating Grand Theft Auto V
- **GTA Modding Community** for research and documentation
- **[v-clothingnames](https://github.com/root-cause/v-clothingnames)** for providing updated JSON data
- **Contributors** who helped improve this tool

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/GenderSwapDump/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/GenderSwapDump/discussions)
- **Documentation**: See `USAGE_GUIDE.md` for detailed usage instructions

## 🔄 Version History

### v2.1.0 (Latest)
- ✅ **Enhanced DLC Resolution**: Smart extraction from `uniqueNameHash` patterns
- ✅ **Code Cleanup**: Removed unused compatibility matrix and shop availability systems
- ✅ **Performance Improvements**: Streamlined processing pipeline
- ✅ **Better Organization**: DLC and component-specific output files
- ✅ **Hash-Based Detection**: More reliable DLC identification

### v2.0.0
- ✅ Unified component system (eliminated redundancy)
- ✅ Performance optimizations (45MB+ log reduction)
- ✅ Enhanced DLC support with proper naming
- ✅ Improved component type detection
- ✅ Better error handling and logging

### v1.5.0
- ✅ Added outfit processing support
- ✅ Enhanced text label extraction
- ✅ Improved gender swap detection
- ✅ Component type mapping improvements

### v1.0.0
- ✅ Initial release
- ✅ Basic XML/JSON parsing
- ✅ Gender swap pair detection
- ✅ Report generation

---

**Made with ❤️ for the GTA V modding community** 