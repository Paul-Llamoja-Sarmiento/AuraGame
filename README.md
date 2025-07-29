# Top-Down RPG (UE5 + GAS)

A **work-in-progress** top-down action RPG built with **Unreal Engine 5** and the **Gameplay Ability System (GAS)**.

---

## 🎮 Demo Preview

👉[Pickup items + Basic HUD](https://youtu.be/FWDv4NXYaKQ)

👉[Attribute Menu](https://youtu.be/z14QqKJltBo)

👉[Point-and-Click + Click-Held movement](https://youtu.be/qP0CTIqUsfQ)

👉[Firebolt spell](https://youtu.be/dKD3aKki6Sw)

---

The goal of this project is to learn and build a flexible RPG foundation that can be expanded with features like inventory, quests, and AI. 
It’s based primarily on the following resources:

- 📺 [Stephen Ulibarri's GAS tutorial](https://www.udemy.com/course/unreal-engine-5-gas-top-down-rpg)
- 📘 [Tranek's GAS Documentation](https://github.com/tranek/GASDocumentation) — A highly recommended deep dive into GAS

---

## 🚧 Current Features (WIP)
- Top-down movement and basic camera
- Enemy highlighting using cursor detection + rendering effects (custom depth)
- Character stats and attributes
- Gameplay Ability System integration
- Item pickup system applying GAS effects and displaying UI messages
- Overlay UI follows an MVC pattern: a controller listens to GAS attribute changes and updates the view (widget) accordingly
- Modular code structure for future scalability

---

## 🚀 Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/Paul-Llamoja-Sarmiento/AuraGame.git

2. Open the .uproject file with Unreal Engine 5.5+

3. Build the project in Visual Studio if required (Ctrl + B)

4. Press Play to run the current demo

## License
This project is for learning and prototyping purposes. License to be defined.
