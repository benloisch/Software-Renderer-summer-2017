#include "Object.h"

Object::Object(int width, int height)
{
	textureWidth = width;
	textureHeight = height;

	textureRGB = new char[width * height * 3];
}

Object::~Object()
{
	delete textureRGB;
}

void Object::loadData(string filename)
{
	ifstream file(filename);
	string data;

	if (file.is_open())
	{
		while (file >> data)
		{
			if (data == "v")
			{
				file >> data;
				double dx = stod(data);
				file >> data;
				double dy = stod(data);
				file >> data;
				double dz = stod(data);
				verticies.push_back(Vector(dx, dy, dz, 1));
			}
			else if (data == "vt")
			{
				file >> data;
				double du = stod(data);
				file >> data;
				double dv = stod(data);
				textureUV.push_back(Vector(du, dv, 0, 0));
			}
			else if (data == "vn")
			{
				file >> data;
				double dnx = stod(data);
				file >> data;
				double dny = stod(data);
				file >> data;
				double dnz = stod(data);
				vertexNormals.push_back(Vector(dnx, dny, dnz, 0));
			}
			else if (data == "f")
			{
				file >> data;

				int index1 = data.find('/');
				int index2 = data.find('/', index1 + 1);

				int num1 = stoi(data.substr(0, index1));
				int num2 = stoi(data.substr(index1 + 1, index2));
				int num3 = stoi(data.substr(index2 + 1, data.size()));

				faces.push_back(Vector(num1, num2, num3, 0));

				file >> data;

				index1 = data.find('/');
				index2 = data.find('/', index1 + 1);

				num1 = stoi(data.substr(0, index1));
				num2 = stoi(data.substr(index1 + 1, index2));
				num3 = stoi(data.substr(index2 + 1, data.size()));

				faces.push_back(Vector(num1, num2, num3, 0));

				file >> data;

				index1 = data.find('/');
				index2 = data.find('/', index1 + 1);

				num1 = stoi(data.substr(0, index1));
				num2 = stoi(data.substr(index1 + 1, index2));
				num3 = stoi(data.substr(index2 + 1, data.size()));

				faces.push_back(Vector(num1, num2, num3, 0));
				
			}
			else
			{
				//do nothing
			}
		}
	}
}

void Object::loadTexture(LPCWSTR filename)
{
	HBITMAP hbmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	if (hbmp == NULL)
	{
		MessageBox(0, L"HBITMAP is null", 0, 0);
	}

	HDC hmem = CreateCompatibleDC(NULL);
	SelectObject(hmem, hbmp);

	for (int y = 0; y < textureHeight; y++)
	{
		for (int x = 0; x < textureWidth; x++)
		{
			COLORREF color;
			color = GetPixel(hmem, x, y);

			textureRGB[(y * textureWidth + x) * 3] = GetBValue(color);
			textureRGB[(y * textureWidth + x) * 3 + 1] = GetGValue(color);
			textureRGB[(y * textureWidth + x) * 3 + 2] = GetRValue(color);
		}
	}

	DeleteObject(hbmp);

}