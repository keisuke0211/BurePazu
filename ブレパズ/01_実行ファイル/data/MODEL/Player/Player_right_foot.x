xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 -4.00000;3.75000;-4.50000;,
 -4.00000;0.75000;-4.50000;,
 -4.00000;0.75000;4.50000;,
 -4.00000;3.75000;4.50000;,
 -2.50000;0.75000;-3.00000;,
 -2.50000;-3.75000;-3.00000;,
 -2.50000;-3.75000;3.00000;,
 -2.50000;0.75000;3.00000;,
 2.00000;-3.75000;-3.00000;,
 2.00000;0.75000;-3.00000;,
 2.00000;-3.75000;3.00000;,
 2.00000;0.75000;3.00000;,
 2.00000;0.75000;-4.50000;,
 2.00000;3.75000;-4.50000;,
 2.00000;0.75000;-3.00000;,
 2.00000;0.75000;3.00000;,
 2.00000;0.75000;4.50000;,
 2.00000;3.75000;4.50000;,
 -2.50000;0.75000;-3.00000;,
 -2.50000;0.75000;3.00000;;
 
 28;
 3;0,1,2;,
 3;3,0,2;,
 3;4,5,6;,
 3;7,4,6;,
 3;8,9,10;,
 3;9,11,10;,
 3;12,13,14;,
 3;15,13,16;,
 3;14,13,15;,
 3;13,17,16;,
 3;12,1,0;,
 3;13,12,0;,
 3;8,5,4;,
 3;9,8,4;,
 3;6,10,7;,
 3;10,11,7;,
 3;2,16,3;,
 3;16,17,3;,
 3;6,5,8;,
 3;10,6,8;,
 3;18,1,12;,
 3;14,18,12;,
 3;19,1,18;,
 3;2,1,19;,
 3;2,19,15;,
 3;16,2,15;,
 3;0,3,13;,
 3;3,17,13;;
 
 MeshMaterialList {
  1;
  28;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\OBJ\\Player.png";
   }
  }
 }
 MeshNormals {
  9;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  -0.000000;0.000000;1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;1.000000;0.000000;;
  28;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;1,1,1;,
  3;1,1,1;,
  3;2,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;1,1,1;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,4,3;,
  3;4,3,3;,
  3;5,6,6;,
  3;6,5,6;,
  3;6,6,6;,
  3;6,6,6;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;8,8,8;,
  3;8,8,8;;
 }
 MeshTextureCoords {
  20;
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.986328;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;,
  0.728516;0.500000;;
 }
}
