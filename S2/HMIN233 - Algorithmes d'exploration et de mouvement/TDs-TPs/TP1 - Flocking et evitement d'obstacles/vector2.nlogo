;; vect est une liste de 2 valeur (x y)

to-report angle-from-vect [vect]
  let a atan item 0  vect item 1 vect
  report a
end

;; Netlogo n'est pas dans le repère classique: le 0 est vers le haut et les angles tournent vers la droite. De ce fait, x = sin angle dans ce référentiel...

to-report vect-from-angle [angle len]
  let l (list (len * sin angle) (len * cos angle))
  report l
end

to-report multiply-scalar-vect [factor vect]
  report (list (item 0 vect * factor) (item 1 vect * factor))
end

to-report add-vect [v1 v2]
  report (list (item 0 v1 + item 0 v2) (item 1 v1 + item 1 v2))
end

to-report length-vect [vect]
  let x item 0 vect
  let y item 1 vect
  report sqrt (x * x) + (y * y)
end