(define (problem task)
(:domain robocluedo)
(:objects
    center wp1 wp2 wp3 wp4 - waypoint
)
(:init

    (not-dirty)

    (is-center center)

    (not-is-center wp1)
    (not-is-center wp2)
    (not-is-center wp3)
    (not-is-center wp4)

    (at wp1)

    (passage center wp1)
    (passage wp1 center)
    (passage center wp4)
    (passage wp4 center)
    (passage wp1 wp2)
    (passage wp2 wp3)
    (passage wp3 wp4)

    (explored wp2)
    (explored wp3)
    (explored wp4)
    (explored wp1)

    (not-explored center)


    (not-at-center)


    (not-hint-ready)

    (hint-collected wp1)

    (not-hint-collected wp4)
    (not-hint-collected wp3)
    (not-hint-collected wp2)

)
(:goal (and
    (dirty)
))
)
