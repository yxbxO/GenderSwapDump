# DLC Update Guide - GTA V Wardrobe Analyzer

## 🎯 Overview

This guide explains how to update the GTA V Wardrobe Analyzer codebase to support new DLC packs as Rockstar Games releases new content. The process is typically straightforward since Rockstar rarely adds new component types - they reuse existing clothing categories.

## 📋 Table of Contents
- [Quick Update Process](#quick-update-process)
- [Understanding DLC Structure](#understanding-dlc-structure)
- [Step-by-Step Update](#step-by-step-update)
- [Testing New DLC](#testing-new-dlc)
- [Deployment](#deployment)

## ⚡ Quick Update Process

**TL;DR**: For most DLC updates, you only need to update **3 things**:

1. **DLC_LIST** in `Core/Constants.h` - Add new DLC entry
2. **XML Files** - Extract from game using OpenIV 
3. **JSON Files** - Download from [v-clothingnames repository](https://github.com/root-cause/v-clothingnames)

**Note**: Component types rarely change - Rockstar reuses existing categories (TORSO, LEGS, SHOES, etc.)

## 🏗️ Understanding DLC Structure

### DLC Naming Conventions

Rockstar follows consistent patterns for DLC naming:

```
Pattern: DLC_MP_[CODE]_[GENDER]_[COMPONENT]_[ID]_[TEXTURE]

Examples:
- DLC_MP_H4_M_TORSO_15_3    (Cayo Perico male torso)
- DLC_MP_SUM24_F_LEGS_8_1   (Summer 2024 female legs)
- DLC_MP_X24_M_OUTFIT_12_0  (Winter 2024 male outfit)
```

### Common DLC Codes
- **H4**: Cayo Perico Heist
- **SUM24**: Summer 2024 Update
- **X24**: Winter 2024 Update  
- **TUNER**: Los Santos Tuners
- **FIXER**: The Contract DLC

### Data Sources
```
Required files come from two sources:
├── XML Files (component definitions)
│   ├── Source: GTA V game files via OpenIV
│   ├── mp_m_freemode_01_mp_m_newdlc_01.xml
│   └── mp_f_freemode_01_mp_f_newdlc_01.xml
└── JSON Files (localized names and metadata)
    ├── Source: https://github.com/root-cause/v-clothingnames
    ├── male_[component].json (e.g., male_tops.json)
    └── female_[component].json (e.g., female_legs.json)
```

## 🔧 Step-by-Step Update

### Step 1: Update Constants.h ⚙️

Add the new DLC to the `DLC_LIST` array in `Core/Constants.h`:

```cpp
// File: GenderSwapDump/Core/Constants.h
// Around line 160-200

constexpr std::array<DlcMapping, NUM_DLC + 1> DLC_LIST = {{  // Increment NUM_DLC
    // ... existing DLC entries ...
    
    // Add new DLC entry
    {"Summer 2025 Update", "DLC_MP_SUM25", "mp2025_01"},
    {"Winter 2025 Update", "DLC_MP_X25", "mp2025_02"},
    {"New Heist Update", "DLC_MP_H5", "mpheist5"},
}};
```

**Important**: 
1. Increment `NUM_DLC` constant: `constexpr size_t NUM_DLC = 49;` → `50`
2. Follow the naming pattern: `{Display Name, DLC Key, Filename}`
3. Keep chronological order (newest at bottom)

### Step 2: Extract XML Files Using OpenIV 📁

1. **Open OpenIV** and navigate to GTA V installation
2. **Browse to**: `x64v.rpf/models/cdimages/streamedpeds_mp.rpf/`
3. **Look for new DLC files**:
   ```
   mp_m_freemode_01_mp_m_[newdlc]_01.xml
   mp_f_freemode_01_mp_f_[newdlc]_01.xml
   ```
4. **Extract to**: `GenderSwapDump/xmlfiles/`

**Example for Summer 2025 DLC**:
```
Extract these files:
- mp_m_freemode_01_mp_m_sum25_01.xml
- mp_f_freemode_01_mp_f_sum25_01.xml
```

### Step 3: Download JSON Files from v-clothingnames 📥

1. **Visit**: [https://github.com/root-cause/v-clothingnames](https://github.com/root-cause/v-clothingnames)
2. **Download updated files** (the repository is regularly updated with new DLC content):
   ```
   Required files:
   - male_[component].json files (e.g., male_tops.json, male_legs.json)
   - female_[component].json files (e.g., female_tops.json, female_legs.json)
   - masks.json, masks_male.json, masks_female.json
   - props_[gender]_[type].json files
   ```
3. **Place in**: `GenderSwapDump/Jsonfiles/`

**Note**: The v-clothingnames repository provides GXT entries and human-readable names for all GTA V clothing items, saving you from manual extraction.

## 🧪 Testing New DLC

### Step 1: Prepare Test Data

Create a minimal test dataset:

```
test_data/
├── xmlfiles/
│   ├── mp_m_freemode_01_mp_m_newdlc_01.xml  (5-10 test items)
│   └── mp_f_freemode_01_mp_f_newdlc_01.xml  (5-10 test items)
├── Jsonfiles/
│   └── test_newdlc.json                      (metadata for test items)
└── scriptmetadata_test.meta                  (small subset with known pairs)
```

### Step 2: Validate DLC Recognition

```cpp
// Add temporary debug code to main() in Source.cpp
int main() {
    // Test new DLC recognition
    std::string testDlc = "DLC_MP_SUM25";
    auto displayName = Core::ComponentUtils::getDlcDisplayName(testDlc);
    std::cout << "Test DLC: " << testDlc << " -> " << displayName << std::endl;
    
    // Expected output: "Test DLC: DLC_MP_SUM25 -> Summer 2025 Update (mp2025_01)"
    
    // ... rest of main function ...
}
```

### Step 3: Run Test Suite

```bash
# Build debug version
cd x64/Debug

# Run with test data
./GenderSwapDump.exe

# Check output for:
# 1. Correct DLC names in reports
# 2. New component types recognized
# 3. Gender swap pairs detected
# 4. No crashes or errors
```

### Step 4: Validate Output

Expected test results:
```
GENDER SWAP STATISTICS:
======================
Summer 2025 Update (mp2025_01): 5 pairs found  ← Should show new DLC
Total New DLC Items: 10
```

## 📦 Deployment

### Version Control Best Practices

#### 1. Simple Update Process
```bash
# Create feature branch for DLC update
git checkout -b feature/dlc-summer2025-support

# Make changes (usually just these 3 steps):
git add Core/Constants.h xmlfiles/ Jsonfiles/
git commit -m "Add Summer 2025 DLC support (DLC_MP_SUM25)"

# Test and merge
git checkout main
git merge feature/dlc-summer2025-support
```

#### 2. Version Tagging
```bash
# Tag new version after DLC update
git tag -a v2.1.0 -m "Added Summer 2025 DLC support"
git push origin v2.1.0
```

### Update Documentation

#### Update README.md
```markdown
### DLC Support
The tool supports all major GTA V DLC packs including:
- Summer 2025 Update (mp2025_01)  ← Add new entry
- Winter 2024 Update (mp2024_02)
- ...
```

## 📊 Monitoring & Maintenance

### Update Checklist

When a new GTA V update is released:

- [ ] **Check Rockstar Newswire** for DLC announcement
- [ ] **Wait for v-clothingnames update** (usually updated quickly by community)
- [ ] **Extract XML files** using OpenIV from game files
- [ ] **Download updated JSON files** from v-clothingnames repository
- [ ] **Update Constants.h** with new DLC information
- [ ] **Run test** with new data
- [ ] **Update documentation** and version numbers
- [ ] **Create GitHub release** with changelog

### Data Source Monitoring

#### v-clothingnames Repository
- **URL**: [https://github.com/root-cause/v-clothingnames](https://github.com/root-cause/v-clothingnames)
- **Update Frequency**: Usually updated within days of new DLC releases
- **Contents**: Complete GXT entries and localized names for all clothing items
- **File Structure**: Organized by component type and gender (e.g., `male_tops.json`, `female_legs.json`)

#### OpenIV for XML Extraction
- **Tool**: OpenIV (free GTA V modding tool)
- **Path**: `x64v.rpf/models/cdimages/streamedpeds_mp.rpf/`
- **Files**: `mp_[gender]_freemode_01_mp_[gender]_[dlc]_01.xml`
- **Update**: Extract after each GTA V game update

---

## 🎯 Quick Reference

### Essential Files to Update (99% of DLC updates)
1. **`Core/Constants.h`** - Add new DLC to DLC_LIST
2. **`xmlfiles/`** - Extract new XML files using OpenIV
3. **`Jsonfiles/`** - Download updated files from v-clothingnames

### Rarely Needed Updates
- **Component detection logic** - Only if Rockstar introduces entirely new clothing categories (rare)
- **Component name cleaning** - Only if new component types are added
- **Parser logic** - Usually works with existing patterns

### Common DLC Patterns
- **Summer Updates**: `DLC_MP_SUM[YY]` → `mp20[YY]_01`
- **Winter Updates**: `DLC_MP_X[YY]` → `mp20[YY]_02` 
- **Heist Updates**: `DLC_MP_H[N]` → `mpheist[N]`
- **Themed Updates**: `DLC_MP_[THEME]` → `mp[theme]`

### Community Resources
- **v-clothingnames**: [https://github.com/root-cause/v-clothingnames](https://github.com/root-cause/v-clothingnames) - Your primary JSON data source
- **OpenIV**: Download from GTAMods.com for XML extraction
- **GTA Forums**: Community discussions about new DLC content

For questions or assistance, open an issue on GitHub. 