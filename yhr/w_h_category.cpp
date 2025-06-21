#include "w_h_category.h"
#include "ui_w_h_category.h"

w_h_category::w_h_category(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::w_h_category)
{
    ui->setupUi(this);

    this->categories = {"치킨", "도시락", "버거", "구이", "샐러드",
                        "한식", "분식", "돈까스", "족발/보쌈", "찜/탕",
                        "피자", "중식", "일식", "회/해물", "양식",
                        "디저트", "간식", "아시안", "샌드위치", "멕시칸",
                        "커피/차", "죽", "포장", "1인분"};

    QMap<QString, QString> categoryImageMap = {
        {"치킨", "CHICKEN.jpeg"},
        {"도시락", "BENTTO.jpeg"},
        {"버거", "BURGER.jpeg"},
        {"구이", "GOOEE.jpeg"},
        {"샐러드", "SALAD.jpeg"},
        {"한식", "HANSIK.jpeg"},
        {"분식", "BOONSIK.jpeg"},
        {"돈까스", "DONGASS.jpeg"},
        {"족발/보쌈", "JOKBAL.jpeg"},
        {"찜/탕", "JJIM_TANG.jpeg"},
        {"피자", "PIZZA.jpeg"},
        {"중식", "CHINA.jpeg"},
        {"일식", "JAPAN.jpeg"},
        {"회/해물", "SEAFOOD.jpeg"},
        {"양식", "USA.jpeg"},
        {"디저트", "DESSERT.jpeg"},
        {"간식", "SNACK.jpeg"},
        {"아시안", "ASIAN.jpeg"},
        {"샌드위치", "SANDWICH.jpeg"},
        {"멕시칸", "MEXICO.jpeg"},
        {"커피/차", "COFFEE.jpeg"},
        {"죽", "JOOK.jpeg"},
        {"포장", "TAKE_OUT.jpeg"},
        {"1인분", "ONE.jpeg"}
    };

    // 스크롤 영역 설정
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget *container = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10); // 간격 조정

    for (int i = 0; i < categories.size(); ++i) {
        QString categoryName = categories[i];
        QString imageName = categoryImageMap[categoryName];

        QToolButton* btn = new QToolButton;
        btn->setIcon(QIcon(":/CATEGORY_IMG/" + imageName));
        btn->setIconSize(QSize(72, 43));
        btn->setText(categoryName);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setStyleSheet("QToolButton { border: none; background-color: transparent; }");
        btn->setFixedSize(SIZE_, SIZE_);

        layout->addWidget(btn);
    }

    scrollArea->setWidget(container);
    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    // 가로 스크롤 숨김

}


w_h_category::~w_h_category()
{
    delete ui;
}
