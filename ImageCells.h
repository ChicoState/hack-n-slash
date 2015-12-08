#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class ImageCells
{
public:
	ImageCells(int rows, int cols, int width, int height);
	virtual ~ImageCells() {}

	int width() const { return m_width; }
	int height() const { return m_height; }
	int cols() const { return m_cols; }
	int rows() const { return m_rows; }

	void setCell(int col, int row, Mat img);
	void setImage(Mat img);
	Mat getCell(int col, int row);
	Mat image;

protected:
	int  m_width;
	int  m_height;
	int  m_cols;
	int  m_rows;
};