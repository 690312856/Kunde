TEMPLATE  = subdirs
SUBDIRS = Main CalculateMethod Communication UI
CalculateMethod.depends = UI
Main.depends = CalculateMethod Communication UI
