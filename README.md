<p align="center">
  <img src="https://cloud.wizurth.ovh/s/ZY9nNWFAKsXmtCY/preview" alt="PGG_HEADER" width="256" height="256">
</p>
<br>
<h1 align="center">Debug Actions System - Core</h1>
<h3>
    A modular Unreal Engine 5 plugin that makes it easy to perform debugging tasks.
</h3>
<h4>
    The DebugActionsSystem makes it easier to use actions such as: <br>
    - Godmode <br>
    - Freecam <br>
    - Add an item to an inventory <br>
    - ... <br>
</h4>

# 📖 Table of Contents
* [How To Start](#-how-to-start)
* [Create Custom Classes](#-custom-class)
   * [Custom Debug Action](#custom-debug-action)
   * [Custom Debug Input](#custom-debug-input)
<br>

<details>
<summary><h2> 🚀 How To Start</h2></summary>

### Step 1
Download the plugin from [master](https://github.com/WizProductions/DebugActionsSystem-Core) branch and place it at _YourProjectFolder/Plugins_, if the "_plugins_" folder does not exist, you must create it.

### Step 2
Launch your project and enable plugin in the "_Plugins_" Window (_Edit/Plugins_).<br>
<img src="https://cloud.wizurth.ovh/s/BXMFqbFoSWPNB2K/preview" alt="DAS_Plugins_Window" width="650" height="150">

### Step 3
Create your data asset inherit from _Debug Actions System Data Asset_.<br>
<img src="https://cloud.wizurth.ovh/s/nriZAXKywnqTg84/preview" alt="DAS_Plugins_Window" width="500" height="150">

### Step 4
Configure your new DataAsset object like the screenshot or with your own subclass.<br>
<img src="https://cloud.wizurth.ovh/s/GeAEQxtJqdt3K4e/preview" alt="DAS_Plugins_Window" width="1100" height="500">

### Step 5
Set the data asset object in the "_Project Settings/Plugins/DebugActionsSystem - Core_".<br>
<img src="https://cloud.wizurth.ovh/s/M93eLWMpHHJaBPr/preview" alt="DAS_Plugins_Window" width="500" height="300">

</details>


# 🧩 Create custom Debug Action