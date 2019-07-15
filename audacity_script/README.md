# Requirements

- Audacity 2.3.2
- Python3

# Run

- Create a Python3 virtual environment, and activate it.

```bash
python3 -m venv venv
source venv/bin/activate
```

- Install dependencies

```bash
pip install -r requirements.txt
```

- Open Audacity, and if mod-script-pipe is not already enabled go to Edit > Preferences > Modules, and select Enabled. Then restart Audacity.

- Run GUI

```bash
python3 GUI.py
```

*Note: SoundFinder arguments don't seem to be passed when using the tool via scripting.
