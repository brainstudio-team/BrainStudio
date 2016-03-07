
# == Associate brn files with brain studio =====================================

# First, create the file  ~/.local/share/mime/packages/application-x-brainstudio.xml:
# Note that you can use any icon, including one for another application
mkdir -p ~/.local/share/mime/packages
cp application-x-brainstudio.xml ~/.local/share/mime/packages

# Next, edit or create the file  ~/.local/share/applications/brainstudio.desktop to contain something like:
#[Desktop Entry]
#Name=BrainStudio
#Exec=/usr/bin/brainstudio
#MimeType=application/x-brainstudio
#Icon=brainstudio
#Terminal=false
#Type=Application
#Categories=AudioVideo;Player;Video;
#Comment=
# Note that Categories should be set appropriately for the application type (in this example, a multimedia app).

# Now update the applications and mime database with:
update-desktop-database ~/.local/share/applications
update-mime-database    ~/.local/share/mime

# Programs that use mime types, such as file managers, should now open *.brn files with brainstudio.

# Restart file manager 
nautilus -q
nautilus

# Adding an Icon
# Now we need to associate an icon with the MIME type. Get an SVG icon and name it "text-extension.svg", or whatever your modified MIME type is named; this will be the icon to represent all instances of the MIME type on your system. For our python example, this is a good choice. Rename the .svg file so that the it matches "text-x-python.svg" (or "insertYourMIMEtype.svg") so that the slashes are replaced with "-" and there are no capital letters.
# Then simply run the following commands, with 'text-x-python' replaced with your MIME type.

sudo cp application-x-brainstudio.svg /usr/share/icons/gnome/scalable/mimetypes
sudo gtk-update-icon-cache /usr/share/icons/gnome/ -f

# Create the desktop file and run:
desktop-file-install --dir=~/.local/share/applications brainstudio.desktop







