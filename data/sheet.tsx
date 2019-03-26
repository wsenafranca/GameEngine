<?xml version="1.0" encoding="UTF-8"?>
<tileset name="sheet" tilewidth="16" tileheight="16" tilecount="136" columns="17">
 <image source="../graphics/sheet.png" width="272" height="128"/>
 <tile id="16" probability="0.95">
  <properties>
   <property name="test" type="int" value="0"/>
  </properties>
  <animation>
   <frame tileid="16" duration="300"/>
   <frame tileid="33" duration="300"/>
  </animation>
 </tile>
 <tile id="33">
  <animation>
   <frame tileid="82" duration="100"/>
   <frame tileid="83" duration="100"/>
   <frame tileid="84" duration="100"/>
  </animation>
 </tile>
</tileset>
