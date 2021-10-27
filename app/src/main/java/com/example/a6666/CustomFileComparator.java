package com.example.a6666;

import java.io.File;
import java.util.Comparator;

public class CustomFileComparator implements Comparator {
    @Override
    public int compare(Object o1, Object o2) {
        File file1 = (File) o1;
        File file2 = (File) o2;

        if (file1.isDirectory()) {
            if (file2.isFile()) {
                return -1;
            }
            return file1.getName().compareTo(file2.getName());
        }
        if (file2.isDirectory()) {
            return 1;
        }
        return file1.getName().compareTo(file2.getName());
    }
}

