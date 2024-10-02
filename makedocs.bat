echo "Generating documentation for the GDExtension demo"
cd demo
godot --doctool ../ --gdextension-docs
cd ..
cd .\doc_classes
del Attribute.xml AttributeBase.xml AttributeBuff.xml AttributeContainer.xml AttributeOperation.xml AttributeSet.xml BuffPoolQueue.xml RuntimeAttribute.xml RuntimeBuff.xml
echo "Documentation generated"
