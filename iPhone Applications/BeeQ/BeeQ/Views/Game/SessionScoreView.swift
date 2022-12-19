//
//  SessionScore.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct SessionScoreView: View {
    @EnvironmentObject var engine: BeeQEngine
    var body: some View {
        HStack {
            Text("Current Score:")
            Text("\(engine.currentSession.sessionScore)")
        }
        .frame(maxWidth: .infinity, alignment: .leading).padding()
    }
}

struct SessionScore_Previews: PreviewProvider {
    static var previews: some View {
        SessionScoreView()
    }
}
