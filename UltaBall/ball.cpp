#include "ball.h"
#include "coreservice.h"

int Ball::idCount = 0;

Ball::Ball(double size)
{
    posx = random() % 600;
    posy = random() % 600;
    dx = random() % 5 - 2;
    dy = random() % 5 - 2;
    id = idCount++;
    this->size = size;
}

Ball::~Ball()
{

}

bool Ball::testCrash(QPointF startPoint, QPointF endPoint) {
    QPointF center = QRect(posx, posy, size, size).center();
    double radius_s = size * size / 4;
    QPointF temp = center - startPoint;
    double centerToStartPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    temp = center - endPoint;
    double centerToEndPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    temp = endPoint - startPoint;
    double endPointToStartPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    double endPointToVerticalPoint_s = centerToEndPoint_s - radius_s;
    double startPointToVerticalPoint_s = centerToStartPoint_s - radius_s;
    if (sqrt(startPointToVerticalPoint_s) + sqrt(endPointToVerticalPoint_s) - sqrt(endPointToStartPoint_s) < 0) {
        return true;
    }
    return false;
}

bool Ball::bounceToLine(QPointF startPoint, QPointF endPoint) {
    QPointF center = QRect(posx, posy, size, size).center();
    double radius_s = size * size / 4;
    QPointF temp = center - startPoint;
    double centerToStartPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    temp = center - endPoint;
    double centerToEndPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    temp = endPoint - startPoint;
    double endPointToStartPoint_s = temp.x() * temp.x() + temp.y() * temp.y();
    double endPointToVerticalPoint_s = centerToEndPoint_s - radius_s;
    double startPointToVerticalPoint_s = centerToStartPoint_s - radius_s;
    if (sqrt(startPointToVerticalPoint_s) + sqrt(endPointToVerticalPoint_s) - sqrt(endPointToStartPoint_s) < 0) {
        temp = endPoint - startPoint;
        QPointF velocity(dx, dy);
        double dotProduct  = QPointF::dotProduct(temp, velocity);
        double velocity_s = dx * dx + dy * dy;
        double cos = dotProduct / (sqrt(dx * dx + dy * dy) * sqrt(temp.x() * temp.x() + temp.y() * temp.y()));
        double verticalVectorLength = sqrt(velocity_s) * sqrt(1.0 - cos * cos) * 2;
        QPointF unitVerticalVertor = QPointF(temp.y() / sqrt(temp.x() * temp.x() + temp.y() * temp.y()), -temp.x() / sqrt(temp.x() * temp.x() + temp.y() * temp.y()));
        /*qDebug() << "cos " << cos;
        qDebug() << "verticalVertorLength" << verticalVectorLength;
        qDebug() << "dx: " << dx << ",dy: " << dy;

        qDebug() << "unit vertical vertor: " << unitVerticalVertor;
        qDebug() << "vertical rate:" << QPointF::dotProduct(temp, unitVerticalVertor);
        */
        QPointF verticalVector = verticalVectorLength * unitVerticalVertor;
        if (QPointF::dotProduct(verticalVector, velocity) > 0)
            verticalVector = -verticalVector;
        velocity += verticalVector;
        dx = velocity.x();
        dy = velocity.y();
        return true;
    } else {
        return false;
    }
}

void Ball::bounceToLines()
{
    CoreService * core = CoreService::getInstance();
    list<Wall> *wallList = core->getWallList();
    for (list<Wall>::iterator it = wallList->begin(); it != wallList->end(); it++) {
        if (testCrash((*it).getStartPoint(), (*it).getEndPoint())) {
            bounceToLine((*it).getStartPoint(), (*it).getEndPoint());
            return;
        }
    }
}

bool Ball::hitTarget(QRect target)
{
    if (QRect(posx, posy, size, size).intersects(target))
        return true;
    return false;
}
int Ball::getId() const
{
    return id;
}

void Ball::setId(int value)
{
    id = value;
}


