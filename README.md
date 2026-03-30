<p align="center">
  <img src="https://cloud.wizurth.ovh/s/ZY9nNWFAKsXmtCY/preview" alt="PGG_HEADER" width="256" height="256">
</p>

<h1 align="center">Debug Actions System - Core</h1>

> **A modular Unreal Engine 5 plugin that makes it easy to perform debugging tasks.**

The **Debug Actions System** makes it easier to use actions such as:
* Godmode
* Freecam
* Add an item to an inventory
* And much more...

---

<img src="https://github.com/user-attachments/assets/ab40b35c-099c-4a6c-84f6-dffdcd438ee1" alt="DAS_Plugins_Window" width="750" height="600">

---

## 📖 Table of Contents
* [🚀 How To Start](#-how-to-start)
* [🧩 Custom Classes](#-custom-classes---blueprint--c)
    * Custom Debug Action
    * Custom Debug Input
* [📘 Example Classes](#-example-classes)

---

## 🚀 How To Start

### Step 1
Download the plugin from the [master](https://github.com/WizProductions/DebugActionsSystem-Core) branch and place it in your `YourProjectFolder/Plugins` directory. *(If the "Plugins" folder does not exist, you must create it).*

### Step 2
Launch your project and enable the plugin in the **Plugins** Window (**Edit > Plugins**).<br>

<img src="https://cloud.wizurth.ovh/s/BXMFqbFoSWPNB2K/preview" alt="DAS_Plugins_Window" width="650" height="150">

### Step 3
Create your Data Asset by inheriting from the `Debug Actions System Data Asset` class.<br>

<img src="https://cloud.wizurth.ovh/s/nriZAXKywnqTg84/preview" alt="DAS_DataAsset_Creation" width="500" height="150">

### Step 4
Configure your new Data Asset object as shown in the screenshot below, or with your own subclasses.<br>

<img src="https://cloud.wizurth.ovh/s/GeAEQxtJqdt3K4e/preview" alt="DAS_DataAsset_Config" width="1100" height="500">

### Step 5
Set the Data Asset object in your Project Settings (**Project Settings > Plugins > DebugActionsSystem - Core**).<br>

<img src="https://cloud.wizurth.ovh/s/M93eLWMpHHJaBPr/preview" alt="DAS_Project_Settings" width="500" height="300">

---

## 🧩 Custom Classes - Blueprint / C++

All classes come with examples included in the plugin. I recommend reviewing them as you create your own custom classes. (See: [📘 Example Classes](#-example-classes)).

<details>
<summary>Custom Debug Action</summary>
<br>

Creating a custom debug action is relatively simple: just create a class that inherits from `UDebugActionBase`.

<img src="https://cloud.wizurth.ovh/s/FKE5HFsT7sWWy5B/preview" alt="DAS_BP_Action_1" width="600" height="175">

In the debug action class, you can override the following methods:

<img src="https://cloud.wizurth.ovh/s/oL48QtkY8yosiwK/preview" alt="DAS_BP_Action_2" width="600" height="175">

| Method | Description |
| :--- | :--- |
| **Execute Debug Action** | The custom behavior triggered when the player clicks the debug action. |
| **GetDebugActionTitle** | Allows you to customize the title of the debug action displayed in the panel. |
| **OnParentFolderIsCollapsed** | Called when the parent folder is collapsed and the debug action becomes hidden. |
| **OnParentFolderIsExpanded** | Called when the parent folder is expanded. This is the ideal location to request and configure inputs. |
| **OnPostInitProperties** | Similar to *BeginPlay*; called after `UDebugAction::PostInitProperties` in C++. |

</details>

<details>
<summary>Custom Debug Input</summary>
<br>

Creating a custom debug input is also relatively simple: just create a class that inherits from `UDebugInputBase`.

<img src="https://cloud.wizurth.ovh/s/D9ZfaApAYB7manT/preview" alt="DAS_BP_Input_1" width="600" height="175">

In the debug input class, you can also override these methods:

<img src="https://cloud.wizurth.ovh/s/k3La5EwakAmRzos/preview" alt="DAS_BP_Input_2" width="650" height="150">

| Method | Description |
| :--- | :--- |
| **Configure Debug Input** | The ideal location to retrieve the widget from the panel and configure its properties. |
| **OnAddedToSlot** | Called when the input is added to its slot and becomes visible to the user. |
| **OnRemovedFromSlot** | Called when the input is removed from its slot and becomes hidden. |

</details>

---

## 📘 Example Classes

Don't forget to enable **Show Plugin Content** in your content browser:

<img src="https://cloud.wizurth.ovh/s/NqWtkrgkyf7NXab/preview" alt="DAS_Show_Plugin_Content" width="600" height="490">

Blueprint example classes can be found at the following locations:

| Example class | Location |
| :--- | :--- |
| **Actions** | `DebugActionsSystemCore/Actions/` |
| **Inputs** | `DebugActionsSystemCore/Inputs/` |
| **DataAssets** | `DebugActionsSystemCore/` |
| **DebugActionWidget** | `DebugActionsSystemCore/UI/` |
| **DebugInputSlotWidget** | `DebugActionsSystemCore/UI/` |
| **DebugPanelMenuWidget** | `DebugActionsSystemCore/UI/` |
<br>

C++ example classes can be found at the following locations:

| Example class | Location |
| :--- | :--- |
| **Actions** | `YourProjectFolder/Plugins/DebugActionsSystemCore/Source/DebugActionsSystemCore/Public/Actions/` |
| **Inputs** | `YourProjectFolder/Plugins/DebugActionsSystemCore/Source/DebugActionsSystemCore/Public/Inputs/` |