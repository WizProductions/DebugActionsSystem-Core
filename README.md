<p align="center">
  <img src="https://cloud.wizurth.ovh/s/ZY9nNWFAKsXmtCY/preview" alt="PGG_HEADER" width="368" height="368">
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
* [Generation Algorithms](#-generation-method-algorithms)
   * [Simple Room Placement](#how-simple-room-placement-works)
   * [BSP](#how-bsp-works)
   * [Cellular Automata](#how-cellular-automata-works)
   * [FastNoiseLite / NoiseLibrary](#how-noiselibrary-works)
* [Credits](#-credits)
<br>

<details>
<summary># 🚀 How To Start</summary>

### Step 1
Download the plugin from [master](https://github.com/WizProductions/DebugActionsSystem-Core) branch and place it at "$YourProjectFolder/Plugins$", if the "plugins" folder does not exist, you must create it.

### Step 2
Launch your project and enable plugin in the "$Plugins$" Window ($Edit/Plguins$).<br>
<img src="https://cloud.wizurth.ovh/s/BXMFqbFoSWPNB2K/preview" alt="DAS_Plugins_Window" width="650" height="150">

### Step 3
Create your data asset inherit from $Debug Actions System Data Asset$.<br>
<img src="https://cloud.wizurth.ovh/s/nriZAXKywnqTg84/preview" alt="DAS_Plugins_Window" width="500" height="150">

### Step 4
Configure your new DataAsset object like the screenshot or with your own subclass.<br>
<img src="https://cloud.wizurth.ovh/s/GeAEQxtJqdt3K4e/preview" alt="DAS_Plugins_Window" width="1100" height="500">

### Step 5
Set the data asset object in the "$Project Settings/Plugins/DebugActionsSystem - Core$".<br>
<img src="https://cloud.wizurth.ovh/s/M93eLWMpHHJaBPr/preview" alt="DAS_Plugins_Window" width="500" height="300">

</details>


# 🧩 Create custom Debug Action


# ✏️ Credits
<a href="https://github.com/Auburn/FastNoiseLite"><img src="https://opengraph.githubassets.com/6afe455114447b1491ef8a727548619d1514ed73574a8f729f2fd8553f355ac4/Auburn/FastNoiseLite" alt="PGG_CREDITS_FAST_NOISE_LITE" width=400 height=200>
