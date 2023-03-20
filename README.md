Creditor
===
This is an Unreal plugin and associated resources to more quickly aid in adding credits to your project. It loads data from a CSV file to generate UMG widgets that can be styled in the editor.

A sibling project for C#/Unity can be found, [here](https://github.com/igda/creditor-unity).

Setup
===

* Clone this repository
* Acquire the submodule source via `git submodule update --init --recursive`
* Add these files to your Unreal project folder
* Add module dependencis to `Creditor` and `CreditorEditor` in your `.uproject`

Usage
===

* Add the [Content/CreditsHUD.uasset](Content/CreditsHUD.uasset) Blueprint to your scene
* Modify the widget templates in [Content/WBP_Credits.uasset](Content/WBP_Credits.uasset)
* Create or modify entry formattings in [Content/Formats](Content/Formats)

Additional Licenses
===

This project uses thirdparty modules whose licenses are listed in their respective submodules. Please review their licensing requirements as to how to include appropriate notices for their use in your project.

About
===

This project is maintained by the IGDA's [Engineering](https://igda.org/sigs/engineering) and [Credits](https://igda.org/sigs/credits) SIGs. To contribute, open a Pull Request or submit an Issue!

Join the discussion on Engineering SIG's [Discord](https://discord.gg/mm6ZHuggaB).
