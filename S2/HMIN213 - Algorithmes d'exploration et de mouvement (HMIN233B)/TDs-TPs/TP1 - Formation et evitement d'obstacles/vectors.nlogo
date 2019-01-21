to-report angleFromVect [vect]
  let a atan item 0  vect item 1 vect
  report a
end

;; Netlogo n'est pas dans le repère classique: le 0 est vers le haut et les angles tournent vers la droite. De ce fait, x = sin angle dans ce référentiel...

to-report vectFromAngle [angle len]
  let l (list (len * sin angle) (len * cos angle))
  report l
end

to-report multiplyScalarvect [factor vect]
  report (list (item 0 vect * factor) (item 1 vect * factor))
end

to-report additionvect [v1 v2]
  report (list (item 0 v1 + item 0 v2) (item 1 v1 + item 1 v2))
end

to retport lengthVect [vect]
  let x item 0 vect
  let y item 1 vectk
  report sqrt (x * x) + (y * y)
end
