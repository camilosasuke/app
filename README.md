# app

app is core repository of **ca2** framework.

ca2 is an all-purpose multi-platform framework and set of libraries written in C++ language to help developers create great apps for users.

## Windows Desktop
### Open Existing Project
#### Assumptions
a. **"helloworld"** project is a project at https://github.com/ca2/app-simple.git repository ( https://github.com/ca2/app-simple/tree/master/appseed/helloworld ). Please, regard it as any project you are willing to work with.

b. **"ca2/app-simple"** repository is the repository where lies "helloworld" project. Please, regard it as any repository and depending repository(ies) you are required to download to work with your project.

#### Procedure
![Folder Structure](/stage/app-simple-folder-structure.png?raw=true "Folder Structure")
1. Create a folder. It will be referred here as **root folder**. Something like C:\test_hello_planet or C:\users\username\lets_start, or any other meaningful name for your workflow. Please, don't use any spaces or special characters, or parent folders with any spaces or special characters. This will avoid issues at compile time.
2. Immediatelly below **root folder**, create new empty folder named **"app"**. 
3. Clone https://github.com/ca2/app.git into this **app** folder.
4. Immediatelly below **root folder**, create new empty folder named **"include"**. 
5. Clone https://github.com/ca2/include.git into this **include** folder.
4. Immediatelly below **root folder**, create new empty folder named **"nodeapp"**. 
5. Clone https://github.com/ca2/nodeapp-windows.git into this **nodeapp** folder. Notice that nodeapp folder doesn't follow convention of naming the folder after exact repository name. The **dash-platform-code-name** suffix, in this case,  **"-windows"**, is stripped from it.
6. Immediatelly below **root folder**, create any new folder(s) related to the project repository(ies). For our simple tutorial, only **"ca2/app-simple"** repository is needed, so create a folder named **"app-simple"**. 
7. Clone to this(these) folder(s), the respective repositories related to the ca2-based app of interest. In our case, clone https://github.com/ca2/app-simple.git into **app-simple** folder.
8. At main project specific folder, in our case, **app-simple**, under **seed/metaseed** subfolder, find desired **"Visual Studio"** solution file to open. In this case, **app_simple_helloworld.sln**.
Enjoy!!

### New Project
#### Assumptions
a. **"heycountry"** is a new project based on the **"helloworld"** project from previous Procedure. Please, regard it as any project with any proper name you are willing to create.

b. **"ca2/app-simple2"** is a local repository or fork repository based on **"app-simple"** repository from previous Procedure. Please, regard it as any repository and depending repository(ies) you are required to download or create to work with your project.

#### Procedure
Repeat steps 1 through 7 from Procedure above for the project and related repositories you want your new project be based on.
Then:
Locate the following subfolders at project specific folder (**app-simple**):
1. **appmatter/helloworld** and copy recursively subfolders and files to **appmatter/heycountry** making proper changes at resource and asset files as desired.

2. **appseed/helloworld** and copy recursively subfolders and files to **appseed/heycountry** making proper changes at project files, like headers and sources, regarding mainly and initially the project name change.

3. **seed/metaseed/app_simple_helloworld.sln** and copy to **seed/metaseed/app_simple_heycountry.sln**.

With just proper renaming, you will be able to open and build your new crafted project.

Built with tbs in <3

