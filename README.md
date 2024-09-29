# myimgui
Test/Learn Dear ImGui

https://github.com/ocornut/imgui

### download code
git clone --recursive

### Add externl git repos as submodules
```
git submodule add https://github.com/ocornut/imgui.git external/imgui

git submodule update --init --recursive
git add .gitmodules external/imgui
git commit -m "Added Dear ImGui as a submodule"
git push


git pull origin master
git add external/imgui
git commit -m "Updated ImGui submodule to latest version"
git push
```

### Download submodule
git submodule update --init --recursive


### Build

make

make test

make test DEBUG=[1](1)
