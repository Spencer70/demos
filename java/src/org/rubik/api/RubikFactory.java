package org.rubik.api;

import org.rubik.impl.Rubik_Impl;

public class RubikFactory {
    private RubikFactory() {}
    public static Rubik create() {
        final RubikFactory factory = new RubikFactory();
        return new Rubik_Impl(factory);
    }
}
