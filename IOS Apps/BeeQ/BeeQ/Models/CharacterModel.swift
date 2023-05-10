//
//  CharacterModel.swift
//  BeeQ
//
//  Created by user224354 on 9/9/22.
//

import Foundation
import SwiftUI

struct CharacterModel : Shape {

    let colors: [Color] = [.yellow, .gray]
    var custSides: Int
    
    init (sides: Int) {
        custSides = sides - 1 // sides are equal to the character number minus 1
    }
    
    func path(in rect: CGRect) -> Path {
        var path = Path()
        let center = CGPoint(x: rect.midX, y: rect.midY)
        let radius = min(rect.size.height, rect.size.width) / 2
        switch custSides {
        case 5:
            let corners = corners(center: center, radius: radius, singleAngle: CGFloat.pi / 2.5)
            path.move(to: corners[0])
            corners[1...4].forEach() { point in path.addLine(to: point)}
        case 6:
            let corners = corners(center: center, radius: radius, singleAngle: -1 * CGFloat.pi / 3)
            path.move(to: corners[0])
            corners[1...5].forEach() { point in path.addLine(to: point)}
        default:
            let corners = corners(center: center, radius: radius, singleAngle: CGFloat.pi / 2)
            path.move(to: corners[0])
            corners[1...3].forEach() { point in path.addLine(to: point)}
        }
        path.closeSubpath()
        return path
    }
    
    func corners(center: CGPoint, radius: CGFloat, singleAngle: CGFloat) -> [CGPoint] {
        var points: [CGPoint] = []
        for i in (0...custSides) {
            let angle = singleAngle * CGFloat(i)
            let point = CGPoint(x: center.x + radius * cos(angle), y: center.y + radius * sin(angle))
            points.append(point)
        }
        return points
    }
    
    func calculateOffset (shapeID: Int, offsetType: Int) -> CGFloat {
        var offset: CGFloat = CGFloat(0)
        var angle: CGFloat = CGFloat(0)
        var einsteinFudge: CGFloat // offset character to get the mirrored side edge look
        switch custSides {
        case 5:
            angle = CGFloat.pi / 2.5 * CGFloat(shapeID)
            einsteinFudge = 132
        case 6:
            angle = CGFloat.pi / 3 * CGFloat(shapeID)
            einsteinFudge = 33
        default:
            angle = CGFloat.pi / 2 * CGFloat(shapeID)
            einsteinFudge = 150
        }
        if shapeID != 0 {
            offset = offsetType == 0 ? 110 * cos(CGFloat(angle + einsteinFudge)) : 110 * sin(CGFloat(angle + einsteinFudge))
            if offsetType == 0 {
                offset += 0
            } else {
                offset += 0
            }
        } else {
            offset = CGFloat(0)
        }
        return offset
        
    } // doc usage : offset type -> x: 0, y: 1
    
}
