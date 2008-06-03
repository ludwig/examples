// http://en.wikipedia.org/wiki/Sierpinski_carpet
import java.awt.*;
import java.applet.*;

public class SierpinskiCarpet extends Applet
{
    private Graphics g = null;
    private int d0 = 729; // 3^6

    public void init()
    {
        g = getGraphics();
        resize(d0,d0);
    }

    public void paint(Graphics g)
    {
        // start recursion
        drawSierpinskiCarpet(0, 0, getWidth(), getHeight());
    }

    private void drawSierpinskiCarpet(int xTL, int yTL, int width, int height)
    {
        if ((width > 2) && (height > 2))
        {
            int w = width/3;
            int h = height/3;
            g.fillRect(xTL + w, yTL + h, w, h);
            for (int k = 0; k < 9; k++)
            {
                if (k != 4)
                {
                    int i = k / 3;
                    int j = k % 3;
                    drawSierpinskiCarpet(xTL + i*w, yTL + j*h, w, h);
                }
            }
        }
    }

}
