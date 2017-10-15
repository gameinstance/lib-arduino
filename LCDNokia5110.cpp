#include <Arduino.h>
#include <LCDNokia5110.h>


LCDNokia5110::LCDNokia5110() {
	// 
	m_SCEPin = 4;
	m_RSTPin = 6;
	m_DCPin = 3;
	m_SDINPin = 11;
	m_SCLKPin = 13;
	m_LEDPin = 10; // via a 330 Ohm resistor
}

LCDNokia5110::LCDNokia5110(unsigned char scePin, unsigned char rstPin, unsigned char dcPin, unsigned char sdinPin, unsigned char sclkPin, unsigned char ledPin) {
	// 
	m_SCEPin = scePin;
	m_RSTPin = rstPin;
	m_DCPin = dcPin;
	m_SDINPin = sdinPin;
	m_SCLKPin = sclkPin;
	m_LEDPin = ledPin; // via a 330 Ohm resistor
}

void LCDNokia5110::Config(unsigned char scePin, unsigned char rstPin, unsigned char dcPin, unsigned char sdinPin, unsigned char sclkPin, unsigned char ledPin) {
	// 
	m_SCEPin = scePin;
	m_RSTPin = rstPin;
	m_DCPin = dcPin;
	m_SDINPin = sdinPin;
	m_SCLKPin = sclkPin;
	m_LEDPin = ledPin; // via a 330 Ohm resistor
}

void LCDNokia5110::Point(int x, int y, bool black) {
	// 
	if ((x < 0) 
		|| (x >= WIDTH) 
		|| (y < 0) 
		|| (y >= HEIGHT)) {
		// coordinates outside the screen area
		return;
	}
	// coordinates within the screen area
	unsigned char shift = y % 8;
	if (black) {
		// black, setting the bit
		m_DisplayMap[x + (y / 8) * WIDTH] |= 1 << shift;
		return;
	}
	// white, resetting the bit
	m_DisplayMap[x + (y / 8) * WIDTH] &= ~(1 << shift);
}

void LCDNokia5110::Line(int x0, int y0, int x1, int y1, bool black) {
	///
	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;
	if (dy < 0) {
		// negative difference on Y axis
		dy = -dy;
		stepy = -1;
	} else {
		// positive difference on Y axis
		stepy = 1;
	}
	if (dx < 0) {
		// negative difference on X axis
		dx = -dx;
		stepx = -1;
	} else {
		// positive difference on X axis
		stepx = 1;
	}
	dy <<= 1;
	dx <<= 1;
	LCDNokia5110::Point(x0, y0, black);
	if (dx > dy) {
		// 
		int fraction = dy - (dx >> 1);
		while (x0 != x1) {
			// 
			if (fraction >= 0) {
				// 
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			LCDNokia5110::Point(x0, y0, black);
		}
	} else {
		// 
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			// 
			if (fraction >= 0) {
				// 
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			LCDNokia5110::Point(x0, y0, black);
		}
	}
}

void LCDNokia5110::Rect(int x0, int y0, int x1, int y1, bool fill, bool black) {
	// 
	if (fill) {
		// filling the rectangle
		int xDiff = (x0 > x1) ? 
			x0 - x1 : 
			x1 - x0;
		while (xDiff > 0) {
			// 
			LCDNokia5110::Line(x0, y0, x0, y1, black);
			if (x0 > x1) {
				// 
				x0 --;
			} else {
				// 
				x0 ++;
			}
			xDiff--;
		}
		return;
	}
	// no fill
	LCDNokia5110::Line(x0, y0, x1, y0, black);
	LCDNokia5110::Line(x0, y1, x1, y1, black);
	LCDNokia5110::Line(x0, y0, x0, y1, black);
	LCDNokia5110::Line(x1, y0, x1, y1, black);
}

void LCDNokia5110::Circle(int x, int y, int radius, bool black, int thickness) {
	// 
	for (int r = 0; r < thickness; r ++) {
		// 
		int f = 1 - radius;
		int ddF_x = 0;
		int ddF_y = -2 * radius;
		int i = 0;
		int j = radius;
		LCDNokia5110::Point(x, y + radius, black);
		LCDNokia5110::Point(x, y - radius, black);
		LCDNokia5110::Point(x + radius, y, black);
		LCDNokia5110::Point(x - radius, y, black);
		while (i < j) {
			// 
			if (f >= 0) {
				// 
				j --;
				ddF_y += 2;
				f += ddF_y;
			}
			i ++;
			ddF_x += 2;
			f += ddF_x + 1;
			LCDNokia5110::Point(x + i, y + j, black);
			LCDNokia5110::Point(x - i, y + j, black);
			LCDNokia5110::Point(x + i, y - j, black);
			LCDNokia5110::Point(x - i, y - j, black);
			LCDNokia5110::Point(x + j, y + i, black);
			LCDNokia5110::Point(x - j, y + i, black);
			LCDNokia5110::Point(x + j, y - i, black);
			LCDNokia5110::Point(x - j, y - i, black);
		}
		radius --;
	}
}

void LCDNokia5110::Char(char character, int x, int y, bool black) {
	// 
	unsigned char column;
	for (int i = 0; i < 5; i ++) {
		// 
		column = ASCII[character - 0x1d][i];
		for (int j = 0; j < 8; j ++) {
			// 
			if (column & (0x01 << j)) {
				// 
				LCDNokia5110::Point(x + i, y + j, black);
			} else {
				// 
				LCDNokia5110::Point(x + i, y + j, !black);
			}
		}
	}
}

void LCDNokia5110::Text(String s, int x, int y, bool black) {
	// 
	return LCDNokia5110::Text(s.c_str(), x, y, black);
}

void LCDNokia5110::Text(const char * dString, int x, int y, bool black) {
	// 
	while (*dString != 0x00) {
		// loop until null terminator
		LCDNokia5110::Char(*dString ++, x, y, black);
		x += 5;
		for (int i = y; i < y + 8; i ++) {
			// 
			LCDNokia5110::Point(x, i, !black);
		}
		x ++;
		if (x > (WIDTH - 5)) {
			// enables wrap around
			x = 0;
			y += 8;
		}
	}
}

void LCDNokia5110::Bitmap(char * bitArray) {
	// 
	for (int i = 0; i < (WIDTH * HEIGHT / 8); i ++) {
		// 
		m_DisplayMap[i] = bitArray[i];
	}
}

void LCDNokia5110::Fill(bool black) {
	// 
	for (int i = 0; i < (WIDTH * HEIGHT / 8); i ++) {
		// 
		m_DisplayMap[i] = black ? 0xFF : 0;
	}
}

void LCDNokia5110::Goto(int x, int y) {
	// 
	LCDNokia5110::Write(0, 0x80 | x);
	LCDNokia5110::Write(0, 0x40 | y);
}

void LCDNokia5110::Update() {
	// 
	LCDNokia5110::Goto(0, 0);
	for (int i = 0; i < (WIDTH * HEIGHT / 8); i ++) {
		// 
		LCDNokia5110::Write(DATA, m_DisplayMap[i]);
	}
}

void LCDNokia5110::Contrast(unsigned char contrast) {
	// 
	LCDNokia5110::Write(COMMAND, 0x21);
	LCDNokia5110::Write(COMMAND, 0x80 | contrast);
	LCDNokia5110::Write(COMMAND, 0x20);
}

void LCDNokia5110::Invert() {
	// 
	LCDNokia5110::Write(COMMAND, 0x20);
	LCDNokia5110::Write(COMMAND, 0x08 | 0x05);
	LCDNokia5110::Write(COMMAND, 0x20);
}

void LCDNokia5110::Write(unsigned char command, unsigned char data) {
	// 
	digitalWrite(m_DCPin, command);
	digitalWrite(m_SCEPin, LOW);
	SPI.transfer(data);
	digitalWrite(m_SCEPin, HIGH);
}

void LCDNokia5110::Light(bool bOn) {
	// 
	analogWrite(m_LEDPin, bOn ? 0 : 255);
}


void LCDNokia5100::DimLight(byte level) {
	// 
	analogWrite(m_LEDPin, level);
}

void LCDNokia5110::Start() {
	// 
	pinMode(m_SCEPin, OUTPUT);
	pinMode(m_RSTPin, OUTPUT);
	pinMode(m_DCPin, OUTPUT);
	pinMode(m_SDINPin, OUTPUT);
	pinMode(m_SCLKPin, OUTPUT);
	pinMode(m_LEDPin, OUTPUT);
	analogWrite(m_LEDPin, 255);
	
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	
	digitalWrite(m_RSTPin, LOW);
	digitalWrite(m_RSTPin, HIGH);
	
	LCDNokia5110::Write(COMMAND, 0x21);
	LCDNokia5110::Write(COMMAND, 0xB0);
	LCDNokia5110::Write(COMMAND, 0x04);
	LCDNokia5110::Write(COMMAND, 0x14);
	LCDNokia5110::Write(COMMAND, 0x20);
	LCDNokia5110::Write(COMMAND, 0x0C);
}
