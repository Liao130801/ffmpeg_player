package com.example.a6666;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.View;

import java.util.Random;

public class autoView extends View {

    public static  final int POINT_NUM=100;
    public static  final int LINE_NUM=2;
    private  String mTitleText;
    private int mTitleTextColor;
    private int mTitleTextSize;
    Random random=new Random();
    private Paint mPaint;
    private Rect mBound;

    String[] mCheckNum = new String[4];
    public autoView(Context context) {
        super(context);
    }

    public autoView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public autoView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        TypedArray a=context.getTheme().obtainStyledAttributes(attrs,R.styleable.autoView,defStyle,0);
        int n = a.getIndexCount();
        for (int i = 0; i < n; i++)
        {
            int attr = a.getIndex(i);
            switch (attr)
            {
                //这个属性可以不要，因为都是随机产生
                case R.styleable.autoView_titleText:
                    mTitleText = a.getString(attr);
                    break;
                case R.styleable.autoView_titleTextColor:
                    // 默认颜色设置为黑色
                    mTitleTextColor = a.getColor(attr, Color.BLACK);
                    break;
                case R.styleable.autoView_titleTextSize:
                    // 默认设置为16sp，TypeValue也可以把sp转化为px
                    mTitleTextSize = a.getDimensionPixelSize(attr, (int) TypedValue.applyDimension(
                            TypedValue.COMPLEX_UNIT_SP, 16, getResources().getDisplayMetrics()));
                    break;

            }

        }
        a.recycle();

        mTitleText = RandomText();

        /**
         * 获得绘制文本的宽和高
         */
        mPaint = new Paint();
        mPaint.setTextSize(mTitleTextSize);
        mBound = new Rect();
        mPaint.getTextBounds(mTitleText, 0, mTitleText.length(), mBound);

        this.setOnClickListener(new OnClickListener()
        {

            @Override
            public void onClick(View v)
            {
                mTitleText = RandomText();
                postInvalidate();
            }

        });



    }

    private String RandomText() {
        StringBuffer sbReturn = new StringBuffer();
        for (int i = 0; i < 4; i++) {
            StringBuffer sb = new StringBuffer();
            int randomInt = random.nextInt(10);
            mCheckNum[i] = sb.append(randomInt).toString();
            sbReturn.append(randomInt);
        }

        return sbReturn.toString();

    }
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        // super.onMeasure(widthMeasureSpec, heightMeasureSpec);

        int width = 0;
        int height = 0;

        /**
         * 设置宽度
         */
        int specMode = MeasureSpec.getMode(widthMeasureSpec);
        int specSize = MeasureSpec.getSize(widthMeasureSpec);
        switch (specMode)
        {
            case MeasureSpec.EXACTLY:// 明确指定了
                width = getPaddingLeft() + getPaddingRight() + specSize;
                break;
            case MeasureSpec.AT_MOST:// 一般为WARP_CONTENT
                width = getPaddingLeft() + getPaddingRight() + mBound.width();
                break;
        }

        /**
         * 设置高度
         */
        specMode = MeasureSpec.getMode(heightMeasureSpec);
        specSize = MeasureSpec.getSize(heightMeasureSpec);
        switch (specMode)
        {
            case MeasureSpec.EXACTLY:// 明确指定了
                height = getPaddingTop() + getPaddingBottom() + specSize;
                break;
            case MeasureSpec.AT_MOST:// 一般为WARP_CONTENT
                height = getPaddingTop() + getPaddingBottom() + mBound.height();
                break;
        }

        setMeasuredDimension(width, height);

    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        //画背景颜色
        mPaint.setColor(Color.BLUE);
        canvas.drawRect(0, 0, getMeasuredWidth(), getMeasuredHeight(), mPaint);

        //划线
        mPaint.setColor(mTitleTextColor);
        int [] line;
        for(int i = 0; i < LINE_NUM; i ++)
        {
            //设置线宽
            mPaint.setStrokeWidth(5);
            line = getLine(getMeasuredHeight(), getMeasuredWidth());
            canvas.drawLine(line[0], line[1], line[2], line[3], mPaint);
        }

        // 绘制小圆点
        int [] point;
        int randomInt;
        for(int i = 0; i < POINT_NUM; i ++)
        {
            //随机获取点的大小
            randomInt = random.nextInt(5);
            point = getPoint(getMeasuredHeight(), getMeasuredWidth());
            canvas.drawCircle(point[0], point[1], randomInt, mPaint);
        }

        //绘制验证控件上的文本
        int dx = 20;
        for(int i = 0; i < 4; i ++){
            canvas.drawText("" + mCheckNum[i],dx, getHeight() / 2 + getPositon(mBound.height() / 2), mPaint);
            dx += (getWidth() / 2 - mBound.width() / 2) + i / 5 + 20;
        }
//		canvas.drawText(mTitleText, getWidth() / 2 - mBound.width() / 2, getHeight() / 2 + mBound.height() / 2, mPaint);
    }

    //计算验证码的绘制y点位置
    private  int getPositon(int height) {
        int tempPositoin = (int) (Math.random() * height);
        if (tempPositoin < 20) {
            tempPositoin += 20;
        }
        return tempPositoin;
    }

    // 随机产生点的圆心点坐标
    public static int[] getPoint(int height, int width) {
        int[] tempCheckNum = { 0, 0, 0, 0 };
        tempCheckNum[0] = (int) (Math.random() * width);
        tempCheckNum[1] = (int) (Math.random() * height);
        return tempCheckNum;
    }

    //随机产生划线的起始点坐标和结束点坐标
    public static int[] getLine(int height, int width) {
        int[] tempCheckNum = { 0, 0, 0, 0 };
        for (int i = 0; i < 4; i += 2) {
            tempCheckNum[i] = (int) (Math.random() * width);
            tempCheckNum[i + 1] = (int) (Math.random() * height);
        }
        return tempCheckNum;
    }
}
