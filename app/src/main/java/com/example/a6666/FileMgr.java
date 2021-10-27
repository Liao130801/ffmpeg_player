package com.example.a6666;

import java.io.File;
import java.io.FilenameFilter;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class FileMgr {
    public List<File> getSubFiles(File file) {
        File[] files = file.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File f, String name) {
                return !name.startsWith(".");
            }
        });
        System.out.println("file:" + (null == files));
        if (null != files) {
            System.out.println("file mgr length:" + files.length);
        }
        List<File> result = Arrays.asList(files);
        Collections.sort(result, new CustomFileComparator());
        return result;
    }
}
