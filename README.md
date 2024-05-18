# ebtask 
**ebtask**: execute background task

```bash
sudo groupadd ebtask 
sudo usermod -aG ebtask your_username
sudo nano /etc/udev/rules.d/99-input.rules
SUBSYSTEM=="input", GROUP="ebtask", MODE="0640"
sudo udevadm control --reload-rules
sudo udevadm trigger
```
