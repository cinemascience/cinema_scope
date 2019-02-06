#include "CinImageView.h"

using namespace std;

CinImageView::CinImageView(QWidget *parent) 
{ 
    // this is the default
    mCurHorParam = "phi";
    mCurVerParam = "theta";
    mCurImage    = "NONE";
}

/*! \brief Given a path to an image, this function loads the image. If the image was loaded previously, then it uses the image from cache. 
 *
 */
bool CinImageView::loadImage(const QString &path)
{
    QPixmap pixmap;
    QPixmap *found = NULL;
    bool result = true;

    // is this the current image?
    if (mCurImage != path) {
        // is it in the cache?
        if (mCache.find(path, found)) {
            // yes, just put it in the scene
            pixmap.load(path);
            mCache.insert(path, pixmap);
            scene()->addPixmap(pixmap);
            mCurImage = path;
            result = true;
            // debug
            // QFileInfo imFile(path);
            // qDebug() << "CINIMAGEVIEW found: " << imFile.fileName();
        } else {
            // no, create a new one
            found = new QPixmap();
            found->load(path);
            mCache.insert(path, *found);
            scene()->addPixmap(*found);
            mCurImage = path;
            result = true;
            // debug
            // QFileInfo imFile(path);
            // qDebug() << "CINIMAGEVIEW load : " << imFile.fileName();
        }
    }

    return result;
}


/*! \brief This slot gets called when a parameter is changed and a new image needs to be loaded. key is ignored for now, since we're only loading one artifact 
 *
 */
void CinImageView::onLoadImage(const QString &key, const QString &path)
{
    loadImage(path);
}


/*! \brief Mouse wheel event. Performs zooming in and out.
 *
 */
void CinImageView::wheelEvent(QWheelEvent * event)
{
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0)
    {
        // Zoom in
        scale(scaleFactor, scaleFactor);

    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

/*! \brief Mouse press event handler.
 *
 */
void CinImageView :: mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"CINIMAGE: on left mouse press"<<endl;
        mCurrentXloc = e->pos().rx();
        mCurrentYloc = e->pos().ry();
    }
}

/*! \brief Mouse release event handler.
 *
 */
void CinImageView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cout<<"CINIMAGE: n left mouse release"<<endl;
        mLastXloc = e->pos().rx();
        mLastYloc = e->pos().ry();
    }
}

/*! \brief Mouse move event handler. This controls the dragging event.
 *
 */
void CinImageView::mouseMoveEvent(QMouseEvent *e)
{
    QPoint p = e->pos();

    // only the horizontal parameter
    if(fabs(p.rx() - mCurrentXloc) > mSlidePixelHor && fabs(p.ry() - mCurrentYloc) < mSlidePixelHor)
    {
        if((p.rx() - mCurrentXloc) > 0 && mLastXloc < p.rx()) //slide right
        {
            mParamSet->increment(mCurHorParam);
        } else if((p.rx() - mCurrentXloc) < 0 && mLastXloc > p.rx()) //slide left
        {
            mParamSet->decrement(mCurHorParam);
        }
    }
    // only the vertical parameter
    else if(fabs(p.ry() - mCurrentYloc) > mSlidePixelVer && fabs(p.rx() - mCurrentXloc) < mSlidePixelVer)
    {

        if((p.ry() - mCurrentYloc) > 0 && mLastYloc < p.ry()) //slide up
        {
            mParamSet->increment(mCurVerParam);
        }

        else if((p.ry() - mCurrentYloc) < 0 && mLastYloc > p.ry()) //slide down
        {
            mParamSet->decrement(mCurVerParam);
        }
    }

    mLastXloc = p.rx();
    mLastYloc = p.ry();
}

void CinImageView::clear()
{
    scene()->clear();
}
