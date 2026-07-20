<p align="center">
  <img src="https://cloud.wizurth.ovh/s/ZY9nNWFAKsXmtCY/preview" alt="PGG_HEADER" width="256" height="256">
</p>
<p align="center"> <a href="https://www.codefactor.io/repository/github/wizproductions/debugactionssystem-core/overview/master">
    <img src="https://www.codefactor.io/repository/github/wizproductions/debugactionssystem-core/badge/master" alt="CodeFactor" />
</a> </p>
<h1 align="center">Debug Actions System - Core</h1>

> **A powerful, modular, and highly extensible debugging framework for Unreal Engine 5.**

The primary goal of **Debug Actions System** is to simplify in-game debugging to a single click, providing absolute flexibility to trigger custom behaviors and instantly supply them with the exact data inputs they need.

Instead of memorizing obscure console commands or hardcoding temporary cheat UI widgets for every new feature, this plugin allows you to easily bind C++ logic or Blueprints to visual debug triggers driven by simple, intuitive information inputs.

---

<img src="https://github.com/user-attachments/assets/ab40b35c-099c-4a6c-84f6-dffdcd438ee1" alt="DAS_Plugins_Window" width="750" height="600">

## 📖 Table of Contents
* [🚀 Key Features](#-key-features)
* [🏗️ Architecture Overview](#️-architecture-overview)
* [🪵 Advanced Logging System](#-advanced-logging-system)
* [🏁 How To Start](#-how-to-start)
* [🧩 Custom Classes - Blueprint / C++](#-custom-classes---blueprint--c)
* [📘 Example Classes](#-example-classes)

---

## 🚀 Key Features

* **One-Click Execution:** Trigger complex debug behaviors instantly from a unified interface (e.g., Godmode, Freecam, Level Loading).
* **Information Inputs:** Easily pass specific data directly to your actions (like selecting a Level asset, choosing an Actor, or moving a slider) without any manual text typing.
* **Highly Modular Framework:** Fully extensible structure allowing you to create your own custom actions and inputs from scratch by inheriting from base classes.
* **Multiplayer & Local Testing Utilities:** Includes built-in actions (like Seamless Travel or network role logs) designed to speed up local testing and quickly debug basic multiplayer flows.

---

## 🏗️ Architecture Overview

The framework is built using a decoupled, modular approach separating state management, execution logic, and UI rendering.

```text
DebugActionsSystem/
├── 📁 SubSystems/   # Global manager & API entry point
├── 📁 Actions/      # Debug behaviors and execution triggers (e.g., Open Level)
├── 📁 Inputs/       # Information inputs required by actions (e.g., Target Level)
├── 📁 WidgetBases/  # Base UI components for modular rendering
└── 📁 Settings/     # Project-wide global configurations
```

### 🔹 Core Components

* **`DebugSubsystem`**: The central backbone of the plugin. This subsystem manages the debug actions system logic.
* **`Debug Actions`**: Execute the debug behaviors and logic (e.g., `FreeCam`, `SeamlessTravel`, `OpenLevel`). Also, the `DebugAction_CommandExecutor` makes it very easy to create Unreal command aliases and macros and allows you to execute them with a single click.
* **`Debug Inputs`**: Provide the required information inputs directly to the actions (e.g., passing a specific Level asset or an Actor instance selector) to handle data context without manual typing.
* **`UI Layer`**: The foundational UMG widget bases that connect your user interface to the Debug Actions System, with pre-configured Blueprint versions already included for instant out-of-the-box usage.
* **`Configuration`**: Native developer settings exposed via the Unreal project settings panel for global customization.

---

## 🪵 Advanced Logging System

**DebugActionsSystem** includes a macro-driven logging system (`DAS_LOG`) designed to optimize debugging workflows by auto-injecting diagnostic context while maintaining **zero overhead in Production**.

### Key Advantages:
* **Rich Context Injection:** Every log automatically captures and displays the exact calling function signature and file line number.
* **Viewport Notification Support:** Built-in hooks to simultaneously dispatch logs onto the screen with custom durations and colors.
* **Network Context Aware:** Specialized network macros (`DAS_NET_LOG`) automatically resolve and prepend the exact networking role context (e.g., `[ROLE_Authority/ROLE_SimulatedProxy]`) by dynamically evaluating the calling `AActor` or `UActorComponent` ownership state.
* **Zero Shipping Overhead:** Macros completely evaluate to empty statements or simple immediate returns in **SHIPPING** builds, ensuring no performance cost or leaked diagnostic logs.

### Available Macros:

```cpp
// Standard rich logging
DAS_LOG("Player health updated to 100", Log, LogTemp);

// Log and instantly return a value (Ideal for early-exits and guard clauses)
DAS_RET_LOG(false, "Failed to locate target Actor component", Error);

// Network-aware logging (Auto-prepends Local/Remote role context: [LocalRole/RemoteRole])
DAS_NET_LOG("Replicated movement RPC received", Display);

// Network-aware logging with immediate return execution
DAS_NET_RET_LOG(nullptr, "Weapon instance not found on client", Warning);
```

---

## 🏁 How To Start

### Step 1 - Installation
**<u>Project version:</u>**<br>
Download the plugin from the [master](https://github.com/WizProductions/DebugActionsSystem-Core) branch and place it in your `YourProjectFolder/Plugins` directory. *(If the "Plugins" folder does not exist, you must create it).*

**<u>Engine version:</u>**<br>
Download the plugin from the [Releases](https://github.com/WizProductions/DebugActionsSystem-Core/releases) and place it in the **Engine**'s plugin folder to easily enable it in all projects. In the `UE_5x/Engine/Plugins/Marketplace` directory.

### Step 2 - Activation
Launch your project and enable the plugin in the **Plugins** Window (**Edit > Plugins**).<br>

<img src="https://cloud.wizurth.ovh/s/BXMFqbFoSWPNB2K/preview" alt="DAS_Plugins_Window" width="650" height="150">

### Step 3 - Data Asset
Create your Data Asset by inheriting from the `Debug Actions System Data Asset` class.<br>

<img src="https://cloud.wizurth.ovh/s/nriZAXKywnqTg84/preview" alt="DAS_DataAsset_Creation" width="500" height="150">

### Step 4 - Configuration
Configure your new Data Asset object as shown in the screenshot below, or with your own subclasses.<br>

<img src="https://cloud.wizurth.ovh/s/GeAEQxtJqdt3K4e/preview" alt="DAS_DataAsset_Config" width="1100" height="500">

### Step 5 - Project Settings
Set the Data Asset object in your Project Settings (**Project Settings > Plugins > DebugActionsSystem - Core**).<br>

<img src="https://cloud.wizurth.ovh/s/M93eLWMpHHJaBPr/preview" alt="DAS_Project_Settings" width="500" height="300">

---

## 🧩 Custom Classes - Blueprint / C++

I highly recommend reviewing the included examples to easily create your own custom actions and inputs. (See: [📘 Example Classes](#-example-classes)).

<details>
<summary><b>Custom Debug Action</b></summary>

### <u>How to create a custom DebugAction</u>:

Creating a custom debug action is relatively simple: just create a class that inherits from `UDebugActionBase`.
<img src="https://cloud.wizurth.ovh/s/FKE5HFsT7sWWy5B/preview" alt="DAS_BP_Action_1" width="600" height="175">

In the debug action class, you can override the following methods:
<img src="https://cloud.wizurth.ovh/s/oL48QtkY8yosiwK/preview" alt="DAS_BP_Action_2" width="600" height="175">

| Method                        | Description                                                                                                            |
|:------------------------------|:-----------------------------------------------------------------------------------------------------------------------|
| **OnExecuteDebugAction**      | The custom behavior triggered when the player clicks the debug action.                                                 |
| **GetDebugActionTitle**       | Allows you to customize the title of the debug action displayed in the panel and in the DataAsset.                     |
| **OnParentFolderIsCollapsed** | Called when the parent folder is collapsed and the debug action becomes hidden.                                        |
| **OnParentFolderIsExpanded**  | Called when the parent folder is expanded. This is the ideal location to request and configure inputs.                 |
| **OnPostInitProperties**      | Similar to *BeginPlay* but called very early after object creation; exposed from `UObject::PostInitProperties` in C++. |
</details>

<details>
<summary><b>Custom Debug Input</b></summary>

### <u>How to create a custom DebugInput</u>:

Creating a custom debug input is also relatively simple: just create a class that inherits from `UDebugInputBase`.
<img src="https://cloud.wizurth.ovh/s/D9ZfaApAYB7manT/preview" alt="DAS_BP_Input_1" width="600" height="175">

In the debug input class, you can also override these methods:
<img src="https://cloud.wizurth.ovh/s/k3La5EwakAmRzos/preview" alt="DAS_BP_Input_2" width="650" height="150">

| Method                    | Description |
|:--------------------------| :--- |
| **OnConfigureDebugInput** | The ideal location to retrieve the widget from the panel and configure its properties. |
| **OnAddedToSlot**         | Called when the input is added to its slot and becomes visible to the user. |
| **OnRemovedFromSlot**     | Called when the input is removed from its slot and becomes hidden. |
</details>

---

## 📘 Example Classes

<details>
<summary>BP_DebugAction_ObjectCounter</summary>
<i>A debug action that counts how many objects of a specific class currently exist in the world, then displays the total count using a log message.</i>
<br>
<br>
<i>GetDebugActionTitle:</i>
<img src="https://cloud.wizurth.ovh/s/JiWBjnrCMXPzS7f/preview" alt="DAS_Example_ObjectCounter_0">
<br><br>
<i>OnExecuteDebugAction:</i>
<img src="https://cloud.wizurth.ovh/s/MPgaFKmEJLZjk52/preview" alt="DAS_Example_ObjectCounter_1">
<br><br>
<i>OnParentFolderIsDeveloped:</i>
<img src="https://cloud.wizurth.ovh/s/cr3bAp9xXcLzSSG/preview" alt="DAS_Example_ObjectCounter_2">
<br><br>
<i>Properties:</i>
<img src="https://cloud.wizurth.ovh/s/jXYwxjnDXcGdzFZ/preview" alt="DAS_Example_ObjectCounter_3">
<br><br>
<i>DataAsset preview:</i>
<img src="https://cloud.wizurth.ovh/s/DwJtdW3AtPA3b4S/preview" alt="DAS_Example_ObjectCounter_4">
</details>

<details>
<summary>BP_DebugInput_TextET</summary>
<i>An input field that retrieves a character string entered by the user as text.</i>
  <br>
<br>
<i>OnConfigureDebugInput:</i>
<img src="https://cloud.wizurth.ovh/s/S5rX3fmREQbqQS5/preview" alt="DAS_Example_StringET_0">
<br><br>
<i>GetValue:</i>
<img src="https://cloud.wizurth.ovh/s/Agc4KLjSE7oQf5R/preview" alt="DAS_Example_StringET_1">
<br><br>
<i>Setup (Optional):</i>
<img src="https://cloud.wizurth.ovh/s/eK5EaSddFmsDjQa/preview" alt="DAS_Example_StringET_2">
</details>

<br>

### Exploring the Built-in Examples

To better understand how these actions and custom entries work together, you can check out the code examples provided directly in the plugin's `/Plugins/Content` folder. This is the fastest way to learn how to create your own.

Don't forget to enable **Show Plugin Content** in your content browser:
<img src="https://cloud.wizurth.ovh/s/NqWtkrgkyf7NXab/preview" alt="DAS_Show_Plugin_Content" width="600" height="490">

**Blueprint example classes:**

| Example class | Location |
| :--- | :--- |
| **Actions** | `DebugActionsSystemCore/Actions/` |
| **Inputs** | `DebugActionsSystemCore/Inputs/` |
| **DataAssets** | `DebugActionsSystemCore/` |
| **DebugActionWidget** | `DebugActionsSystemCore/UI/` |
| **DebugInputSlotWidget** | `DebugActionsSystemCore/UI/` |
| **DebugPanelMenuWidget** | `DebugActionsSystemCore/UI/` |

**C++ example classes:**

| Example class | Location                                                                                        |
| :--- |:------------------------------------------------------------------------------------------------|
| **Actions** | `/Plugins/DebugActionsSystemCore/Source/DebugActionsSystemCore/Public/Actions/`                 |
| **Inputs** | `/Plugins/DebugActionsSystemCore/Source/DebugActionsSystemCore/Public/Inputs/`